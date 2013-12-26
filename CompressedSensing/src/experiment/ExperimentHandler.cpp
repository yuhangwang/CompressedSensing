#include "src/experiment/ExperimentHandler.h"
#include <Windows.h>
#include <chrono>
#include "src/utils/utils.h"
#include "src/utils/log.h"

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace CS::experiment;
using namespace CS::camera;

ExperimentHandler::ExperimentHandler(std::shared_ptr<ICamera> _camera, ExperimentParameters& params): framesProcessed(0),
	framesToProcess((int)ceil(params.measurementRatio * params.imageHeight * params.imageWidth)), isMeasurementEnded(false), parameters(params.measurementRatio, params.imageWidth, params.imageHeight) {
	this->parameters = params;
	singlePixelCameraOutput = cv::Mat(framesToProcess, 1, CV_32S);
	camera = std::move(_camera);
	camera->registerCallback(std::bind(&ExperimentHandler::simulateSinglePixelCamera, this, std::placeholders::_1));
}

ExperimentHandler::~ExperimentHandler() {
	LOG_DEBUG("");
}

void ExperimentHandler::handleExperiment() {
	gatherMeasurements();
}

//private methods

void ExperimentHandler::gatherMeasurements() {
	measurementMatrix = cv::Mat(framesToProcess, parameters.imageHeight * parameters.imageWidth, CV_8UC1);
	gpuSolver.createBinaryMeasurementMatrix(framesToProcess, parameters.imageHeight * parameters.imageWidth, &measurementMatrix);
	
	int hash = (int)std::this_thread::get_id().hash();
	LOG_DEBUG("this_thread_hash = "<<hash);
	camera->grab();
	waitUntilGrabbingFinished();
	camera->stop();
}

void ExperimentHandler::waitUntilGrabbingFinished() {
	{
		std::unique_lock<std::mutex> lock(measurementMutex);
		cv.wait(lock, [this] {return isMeasurementEnded;});
	}
	LOG_DEBUG("Grabbing finished!");
}

void ExperimentHandler::simulateSinglePixelCamera(Frame& frame) {
	if(framesProcessed < framesToProcess) {
		image = cv::Mat(1, frame.imageHeight * frame.imageWidth, CV_8UC1);
	
		singlePixelCameraOutput.at<int>(framesProcessed,0) = (int)measurementMatrix.row(framesProcessed).dot(image);
		framesProcessed++;

		LOG_DEBUG("hash = "<<(int)std::this_thread::get_id().hash());
		LOG_DEBUG("size measMat.row("<<framesProcessed-1<<") = "<<measurementMatrix.row(framesProcessed-1).size() << "imageTaken size = "<<image.size().height * image.size().width);
		LOG_DEBUG("Frames processed = "<<framesProcessed<<"output["<<framesProcessed-1<<"] = "<<singlePixelCameraOutput.at<int>(framesProcessed-1,0));
		if(framesProcessed >= framesToProcess) {
			notifyMeasurementEnded();
			LOG_DEBUG("Experiment ended. Frames processed = "<<framesProcessed<<" toProcess = "<<framesToProcess);
		}
	}
}

void ExperimentHandler::simpleTransform(Frame& frame) {
	static unsigned long long prevTimestamp = 0, diff = 0;
	image = cv::Mat(frame.imageHeight, frame.imageWidth, CV_8UC1);
	image.data = frame.data;
	diff = frame.timeStamp - prevTimestamp;
	prevTimestamp = frame.timeStamp;
	cv::threshold(image, image, 100, 255, CV_THRESH_BINARY);

	framesProcessed++;
	if(framesProcessed >= framesToProcess) notifyMeasurementEnded();
	LOG_DEBUG("timestamp = "<<frame.timeStamp<<" diff = "<<diff <<"processed no "<<framesProcessed <<"toProcess = "<<framesToProcess);
}

void ExperimentHandler::notifyMeasurementEnded() {
	{
		std::unique_lock<std::mutex> lock(measurementMutex);
		isMeasurementEnded = true;
	}
	cv.notify_one();
}
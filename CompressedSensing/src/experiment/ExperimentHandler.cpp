#include "src/experiment/ExperimentHandler.h"
#include "src/utils/mathUtils.h"

#include <Windows.h>
#include <chrono>
#include "src/utils/utils.h"
#include "src/utils/log.h"

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace CS::experiment;
using namespace CS::camera;
using namespace CS::math;

ExperimentHandler::ExperimentHandler(SmartCameraPtr _camera, SmartAlgorithmPtr _recoverer, SmartSolverPtr _solver, ExperimentParameters& params): framesProcessed(0),
	framesToProcess((int)ceil(params.measurementRatio * params.imageHeight * params.imageWidth)), isMeasurementEnded(false), parameters(params.measurementRatio, params.imageWidth, params.imageHeight) {
	this->parameters = params;
	singlePixelCameraOutput = cv::Mat(framesToProcess, 1, CV_32FC1);
	camera = std::move(_camera);
	recoverer = std::move(_recoverer);
	solver = std::move(_solver);
	camera->registerCallback(std::bind(&ExperimentHandler::simulateSinglePixelCamera, this, std::placeholders::_1));
	LOG_DEBUG("Number of camera references = "<<camera.use_count());
}

ExperimentHandler::~ExperimentHandler() {
	LOG_DEBUG("");
}

void ExperimentHandler::handleExperiment() {
	auto cameraOutput = gatherMeasurements();
	cv::Mat x0 = computeStartingSolution(std::get<0>(cameraOutput), std::get<1>(cameraOutput));
	cv::Mat recoveredImage = recoverer->recoverImage(std::get<0>(cameraOutput), std::get<1>(cameraOutput), x0);
	debugImageShow(recoveredImage);
}

//private methods

void ExperimentHandler::debugImageShow(const cv::Mat& image) {
	cv::Mat draw;
	double maxValue, minValue;
	cv::minMaxLoc(image, &minValue, &maxValue);
	image.convertTo(draw, CV_8U, 255.0/(maxValue - minValue), -minValue *255.0/(maxValue - minValue));
	cv::namedWindow("debug-window", CV_WINDOW_AUTOSIZE);
	cv::imshow("debug-window", draw);

	LOG_DEBUG("Close window to continue program operation");
	cv::waitKey();
}

std::tuple<cv::Mat&, cv::Mat&> ExperimentHandler::gatherMeasurements() {
	measurementMatrix = cv::Mat(framesToProcess, parameters.imageHeight * parameters.imageWidth, CV_32FC1);
	solver->createBinaryMeasurementMatrix(framesToProcess, parameters.imageHeight * parameters.imageWidth, &measurementMatrix);
	
	int hash = (int)std::this_thread::get_id().hash();
	LOG_DEBUG("this_thread_hash = "<<hash);
	camera->grab();
	waitUntilGrabbingFinished();
	camera->stop();

	return std::make_tuple(std::ref(measurementMatrix), std::ref(singlePixelCameraOutput));
}

cv::Mat ExperimentHandler::computeStartingSolution(cv::Mat& measurementMatrix, cv::Mat& cameraOutput) {
	//starting solution = min energy
	LOG_DEBUG("Compute starting solution start");
	cv::Mat returnMatrix = solver->linsolve(measurementMatrix, cameraOutput);

	return returnMatrix;
}

void ExperimentHandler::waitUntilGrabbingFinished() {
	{
		std::unique_lock<std::mutex> lock(measurementMutex);
		cv.wait(lock, [this] {return isMeasurementEnded;});
	}
	LOG_DEBUG("Grabbing finished!");
}

void ExperimentHandler::simulateSinglePixelCamera(const Frame& frame) {
	if(framesProcessed < framesToProcess) {
		image = frame.image.reshape(1, 1); //actually creates a copy. hala OpenCV!
		
		LOG_DEBUG("frame.image size"<<frame.image.size()<<" and internal size " <<image.size()<<"types = "<<frame.image.type()<<" and "<<image.type());
		
		singlePixelCameraOutput.at<float>(framesProcessed,0) = (float)measurementMatrix.row(framesProcessed).dot(image);
		framesProcessed++;

		LOG_DEBUG("hash = "<<(int)std::this_thread::get_id().hash());
		LOG_DEBUG("size measMat.row("<<framesProcessed-1<<") = "<<measurementMatrix.row(framesProcessed-1).size() << "imageTaken size = "<<image.size().height * image.size().width);
		LOG_DEBUG("Frames processed = "<<framesProcessed<<"output["<<framesProcessed-1<<"] = "<<singlePixelCameraOutput.at<float>(framesProcessed-1,0));
		if(framesProcessed >= framesToProcess) {
			notifyMeasurementEnded();
			LOG_DEBUG("Experiment ended. Frames processed = "<<framesProcessed<<" toProcess = "<<framesToProcess);
		}
	}
}

void ExperimentHandler::notifyMeasurementEnded() {
	{
		std::unique_lock<std::mutex> lock(measurementMutex);
		isMeasurementEnded = true;
	}
	cv.notify_one();
}
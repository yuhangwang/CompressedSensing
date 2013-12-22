#include "src/experiment/ExperimentHandler.h"
#include <Windows.h>
#include <thread>
#include <chrono>
#include "src/utils/utils.h"
#include "src/utils/log.h"
#include <vsmc/opencl/urng.h>

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace CS::experiment;
using namespace CS::camera;

ExperimentHandler::ExperimentHandler(std::shared_ptr<ICamera> _camera, ExperimentParameters& params): framesProcessed(0),
	framesToProcess((int)ceil(params.measurementRatio * params.imageHeight * params.imageWidth)), isExperimentEnded(false) {
	this->parameters = params;
	camera = std::move(_camera);
	camera->registerCallback(std::bind(&ExperimentHandler::simpleTransform, this, std::placeholders::_1));
}

ExperimentHandler::~ExperimentHandler() {
	LOG_DEBUG("");
}

void ExperimentHandler::handleExperiment() {
	measurementMatrix = gatherMeasurements();
}

//private methods

cv::Mat& ExperimentHandler::gatherMeasurements() {
	cv::Mat matrix(framesToProcess, parameters.imageHeight * parameters.imageWidth, CV_8UC1);
	


	return matrix;
}

void ExperimentHandler::simpleTransform(Frame& frame) {
	static unsigned long long prevTimestamp = 0, diff = 0;
	image = cv::Mat(frame.imageHeight, frame.imageWidth, CV_8UC1);
	image.data = frame.data;
	diff = frame.timeStamp - prevTimestamp;
	prevTimestamp = frame.timeStamp;
	cv::threshold(image, image, 100, 255, CV_THRESH_BINARY);

	framesProcessed++;
	if(framesProcessed >= framesToProcess) {
		isExperimentEnded = true;
	}
	LOG_DEBUG("timestamp = "<<frame.timeStamp<<" diff = "<<diff <<"processed no "<<framesProcessed <<"toProcess = "<<framesToProcess);
}
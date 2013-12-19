#include "src/experiment/ExperimentHandler.h"
#include <Windows.h>
#include <thread>
#include <chrono>
#include "src/utils/utils.h"

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace CS::experiment;
using namespace CS::camera;

ExperimentHandler::ExperimentHandler(std::shared_ptr<ICamera> _camera, ExperimentParameters& params): framesProcessed(0),
	framesToProcess((int)ceil(params.measurementRatio * params.imageHeight * params.imageWidth)) {
	camera = std::move(_camera);
	camera->registerCallback(std::bind(&ExperimentHandler::simpleTransform, this, std::placeholders::_1));
}

ExperimentHandler::~ExperimentHandler() {
	BOOST_LOG_TRIVIAL(debug) << "ExperimentHandler::~ExperimentHandler\n";
}

void ExperimentHandler::handleExperiment() {
	camera->grab();
	while(framesProcessed < framesToProcess) {
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
	camera->stop();
	BOOST_LOG_TRIVIAL(debug) << "Leaving ExperimentHandler::handleExperiment\n";
}

//private methods

void ExperimentHandler::simpleTransform(Frame& frame) {
	static unsigned long long prevTimestamp = 0, diff = 0;
	image = cv::Mat(frame.imageHeight, frame.imageWidth, CV_8UC1);
	image.data = frame.data;
	diff = frame.timeStamp - prevTimestamp;
	prevTimestamp = frame.timeStamp;
	BOOST_LOG_TRIVIAL(debug) << "timestamp = "<<frame.timeStamp<<" diff = "<<diff<<std::endl;
	cv::threshold(image, image, 100, 255, CV_THRESH_BINARY);

	framesProcessed++;
	BOOST_LOG_TRIVIAL(debug) << "processed no "<<framesProcessed <<"toProcess = "<<framesToProcess;
}
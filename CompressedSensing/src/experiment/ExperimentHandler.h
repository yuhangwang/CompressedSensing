#ifndef EXPERIMENT_HANDLER_H
#define EXPERIMENT_HANDLER_H

#include <memory>
#include <thread>
#include <atomic>
#include <mutex>
#include "src/camera/ICamera.h"
#include <opencv2/core/core.hpp>

namespace CS {
namespace experiment {

struct ExperimentParameters {
public:
	int imageWidth;
	int imageHeight;
	double measurementRatio;
};

class ExperimentHandler {
public:
	ExperimentHandler(std::shared_ptr<camera::ICamera> camera, ExperimentParameters& params);
	~ExperimentHandler();

	void handleExperiment();
private:
	//methods
	void simpleTransform(camera::Frame& frame);

	//data
	int framesProcessed;
	const int framesToProcess;
	std::shared_ptr<camera::ICamera> camera;
	cv::Mat image;
	std::thread experimentThread;
};

}} //namespace end

#endif /*EXPERIMENT_HANDLER_H*/
#ifndef EXPERIMENT_HANDLER_H
#define EXPERIMENT_HANDLER_H

#include <memory>
#include <thread>
#include <atomic>
#include <mutex>
#include "src/camera/ICamera.h"
#include "src/gpu/GPUSolver.h"
#include <opencv2/core/core.hpp>

namespace CS {
namespace experiment {

struct ExperimentParameters {
	ExperimentParameters(double ratio, int width, int height): measurementRatio(ratio), imageWidth(width), imageHeight(height) {}

	double measurementRatio;
	int imageWidth;
	int imageHeight;
};

class ExperimentHandler {
public:
	ExperimentHandler(std::shared_ptr<camera::ICamera> camera, ExperimentParameters& params);
	~ExperimentHandler();

	void handleExperiment();
private:
	//methods
	cv::Mat& gatherMeasurements();
	void simpleTransform(camera::Frame& frame);

	//data
	CS::gpu::GPUSolver gpuSolver;
	int framesProcessed;
	const int framesToProcess;
	ExperimentParameters parameters;
	std::shared_ptr<camera::ICamera> camera;
	std::atomic<bool> isExperimentEnded;
	cv::Mat image;
	cv::Mat measurementMatrix;
	std::thread experimentThread;
};

}} //namespace end

#endif /*EXPERIMENT_HANDLER_H*/
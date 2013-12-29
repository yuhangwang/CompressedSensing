#ifndef EXPERIMENT_HANDLER_H
#define EXPERIMENT_HANDLER_H

#include <memory>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
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
	std::tuple<cv::Mat&, cv::Mat&> gatherMeasurements();
	cv::Mat computeStartingSolution(cv::Mat& measurementMatrix, cv::Mat& cameraOutput);

	void waitUntilGrabbingFinished();
	void simulateSinglePixelCamera(const camera::Frame& frame);

	void notifyMeasurementEnded();

	void debugImageShow(const cv::Mat&);
	//data
	CS::gpu::GPUSolver gpuSolver;
	int framesProcessed;
	const int framesToProcess;
	ExperimentParameters parameters;
	std::shared_ptr<camera::ICamera> camera;
	std::mutex measurementMutex;
	std::condition_variable cv;
	bool isMeasurementEnded;
	cv::Mat image;
	cv::Mat singlePixelCameraOutput;
	cv::Mat measurementMatrix;
	std::thread experimentThread;
};

}} //namespace end

#endif /*EXPERIMENT_HANDLER_H*/
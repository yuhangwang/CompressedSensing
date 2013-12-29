#ifndef TEST_CAMERA_H
#define TEST_CAMERA_H

#include "src/camera/ICamera.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>

namespace CS {
namespace camera {

class TestCamera : public ICamera {
public:

	TestCamera(int imageWidth, int imageHeight);
	~TestCamera();

	void grab();
	void stop();
	void registerCallback(std::function<void (const Frame& frame)> function);

private:
	//methods
	void loadImage();
	void displayImage();
	void processImage();

	void waitForNewFrame();
	void notifyNewFrameReady();
	void resetNewFrameReady();

	//data
	int fps;
	int imageWidth, imageHeight;
	cv::Mat internalPic;
	std::mutex m, dataReadyMutex;
	std::atomic<bool> isGrabbing;
	bool isNewDataReady;
	std::condition_variable dataReadyCV;
	std::thread displayThread, processingThread;
	std::function<void (Frame&)> processingFunction;
};

}}

#endif /*TEST_CAMERA_H*/
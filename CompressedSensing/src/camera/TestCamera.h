#ifndef TEST_CAMERA_H
#define TEST_CAMERA_H

#include "src/camera/ICamera.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <thread>
#include <mutex>
#include <atomic>

namespace CS {
namespace camera {

class TestCamera : public ICamera {
public:

	TestCamera(int imageWidth, int imageHeight);
	~TestCamera();

	void grab();
	void stop();
	void registerCallback(std::function<void (Frame& frame)> function);

private:
	void loadImage();
	void displayImage();
	void processImage();
	Frame matToFrame(cv::Mat& image);
	
	int imageWidth, imageHeight;
	cv::Mat internalPic;
	std::mutex m;
	std::atomic<bool> isGrabbing, isNewDataReady;
	std::thread displayThread, processingThread;
	std::function<void (Frame&)> processingFunction;
};

}}

#endif /*TEST_CAMERA_H*/
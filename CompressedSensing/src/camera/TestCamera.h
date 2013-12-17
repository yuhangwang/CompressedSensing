#ifndef TEST_CAMERA_H
#define TEST_CAMERA_H

#include "src/camera/ICamera.h"
#include <opencv2/core/core.hpp>
#include <opencv/cv.h>

namespace CS {
namespace camera {

class TestCamera : public ICamera {
public:

	TestCamera(int imageWidth, int imageHeight);
	~TestCamera();

	void grab();
	void stop();
	void setCallback(void (*callbackFunction)(void *context));

	cv::Mat& gatherMeasurements();
private:
	cv::Mat outFile;
};

}}

#endif /*TEST_CAMERA_H*/
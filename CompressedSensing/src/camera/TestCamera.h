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
	void registerCallback(std::function<void (Frame& frame)> function);

private:
	cv::Mat outFile;
};

}}

#endif /*TEST_CAMERA_H*/
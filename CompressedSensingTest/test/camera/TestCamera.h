#ifndef TEST_CAMERA_H
#define TEST_CAMERA_H

#include "src/camera/ICamera.h"

class TestCamera : public ICamera {
public:
	TestCamera();
	~TestCamera();

	void snap();
	void grab();
	void stop();
};

#endif /*TEST_CAMERA_H*/
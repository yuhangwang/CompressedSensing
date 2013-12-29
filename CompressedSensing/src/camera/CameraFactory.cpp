#include "src/camera/CameraFactory.h"
#include "src/exceptions/Exceptions.h"
#include "src/utils/utils.h"
#include <string>
#include <iostream>
#include <algorithm>

using namespace CS::camera;
using namespace CS::exception;

ICamera* CameraFactory::getInstance(std::string& type, int imageWidth, int imageHeight) {
	std::transform(type.begin(), type.end(), type.begin(), ::tolower);
	if(type == "jaicamera") {
		return new JaiCamera(imageWidth, imageHeight);
	}else if(type == "testcamera") {
		return new TestCamera(imageWidth, imageHeight);
	}else {
		throw UnknownTypeException(type);
	}
}
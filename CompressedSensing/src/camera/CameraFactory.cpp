#include "src/camera/CameraFactory.h"
#include "src/exceptions/Exceptions.h"
#include "src/utils/utils.h"
#include <string>
#include <iostream>

using namespace CS::camera;
using namespace CS::exception;

ICamera* CameraFactory::getInstance(std::string& type, int imageWidth, int imageHeight) {
	if(type == "JaiCamera") {
		return new JaiCamera(imageWidth, imageHeight);
	}else if(type == "TestCamera") {
		return new TestCamera(imageWidth, imageHeight);
	}else {
		throw UnknownTypeException(type);
	}
}
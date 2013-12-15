#include "src/camera/CameraFactory.h"
#include "src/exceptions/Exceptions.h"
#include "src/utils/utils.h"
#include <string>
#include <iostream>

using namespace CS::camera;
using namespace CS::exception;

ICamera* CameraFactory::getInstance(std::string& type, int imageWidth, int imageHeight, double measurementRatio) {
	if(type == "JaiCamera") {
		BOOST_LOG_TRIVIAL(debug) << "JaiCamera created";
		return new JaiCamera(imageWidth, imageHeight, measurementRatio);
	}else if(type == "TestCamera") {
		BOOST_LOG_TRIVIAL(debug) << "TestCamera created";
		return new TestCamera(imageWidth, imageHeight, measurementRatio);
	}else {
		throw UnknownTypeException(type);
	}
}
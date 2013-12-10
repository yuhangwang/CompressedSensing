#ifndef CAMERA_FACTORY_H
#define CAMERA_FACTORY_H

#include <string>
#include "src/camera/ICamera.h"
#include "src/camera/JaiCamera.h"
#include "src/camera/TestCamera.h"


namespace CS {
namespace camera {

class CameraFactory {
public:
	static ICamera* getInstance(std::string&, int imageWidth, int imageHeight, double measurementRatio);
};

}}

#endif /*CAMERA_FACTORY_H*/
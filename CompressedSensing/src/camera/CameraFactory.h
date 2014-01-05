#ifndef CAMERA_FACTORY_H
#define CAMERA_FACTORY_H

#include <string>
#include "src/camera/ICamera.h"



namespace CS {
namespace camera {

class CameraFactory {
public:
	static ICamera* getInstance(std::string&, int imageWidth, int imageHeight);
};

}}

#endif /*CAMERA_FACTORY_H*/
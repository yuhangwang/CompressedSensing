#ifndef I_CAMERA_H
#define I_CAMERA_H

#include <opencv/cv.h>
#include <opencv2/core/core.hpp>

namespace CS {
namespace camera {

class ICamera 
{
public:
	ICamera();
	virtual ~ICamera()=0;
	
	virtual void grab()=0;
	virtual void stop()=0;

	virtual void setCallback(void (*callbackFunction)(void* context))=0;
};

}}

#endif /*I_CAMERA_H*/
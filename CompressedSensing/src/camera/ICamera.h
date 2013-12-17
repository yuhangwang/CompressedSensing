#ifndef I_CAMERA_H
#define I_CAMERA_H

#include <opencv/cv.h>
#include <opencv2/core/core.hpp>
#include <memory>
#include <functional>

namespace CS {
namespace camera {

struct Frame
{
	Frame(int imageWidth, int imageHeight, unsigned char* data) {
		this->imageWidth = imageWidth;
		this->imageHeight = imageHeight;
		this->data = data;
	}

	int imageWidth;
	int imageHeight;
	unsigned char* data;
};

class ICamera 
{
public:
	ICamera();
	virtual ~ICamera()=0;
	
	virtual void grab()=0;
	virtual void stop()=0;

	virtual void registerCallback(std::function<void (Frame& frame)>)=0;
};

}}

#endif /*I_CAMERA_H*/
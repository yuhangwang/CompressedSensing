#ifndef I_CAMERA_H
#define I_CAMERA_H

#include <opencv/cv.h>
#include <opencv2/core/mat.hpp>
#include <memory>
#include <functional>
#include <opencv2/opencv.hpp>

namespace CS {
namespace camera {

struct Frame {
	Frame(cv::Mat matrix, unsigned long long timeStamp) {
		this->image = matrix;
		this->timeStamp = timeStamp;
	}
	cv::Mat image;
	unsigned long long timeStamp;
};

class ICamera 
{
public:
	ICamera();
	virtual ~ICamera()=0;
	
	virtual void grab()=0;
	virtual void stop()=0;

	virtual void registerCallback(std::function<void (const Frame& frame)>)=0;
};

}}

#endif /*I_CAMERA_H*/
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
	
	virtual cv::Mat& gatherMeasurements()=0;
};

}}

#endif /*I_CAMERA_H*/
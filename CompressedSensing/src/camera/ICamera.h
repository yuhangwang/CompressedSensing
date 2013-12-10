#ifndef I_CAMERA_H
#define I_CAMERA_H

namespace CS {
namespace camera {

class ICamera 
{
public:
	ICamera();
	virtual ~ICamera()=0;
	
	virtual void snap()=0;
	virtual void grab()=0;
	virtual void stop()=0;
};

}}

#endif /*I_CAMERA_H*/
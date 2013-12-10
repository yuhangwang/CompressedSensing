#ifndef JAI_CAMERA_H
#define JAI_CAMERA_H

#include "src/camera/ICamera.h"
#include <string>
#include <stdint.h>
#include <Jai_Factory.h>

namespace CS {
namespace camera {

class JaiCamera : public ICamera {
public:
	JaiCamera(int, int, double);
	~JaiCamera();

	void snap();
	void grab();
	void stop();

private:
	CAM_HANDLE camHandle;
	FACTORY_HANDLE factoryHandle;
	VIEW_HANDLE viewHandle;
	THRD_HANDLE hThread;

	int64_t GetParameter(std::string paramName);
	int64_t GetSizeOfBuffer();

	void validator(const char *, J_STATUS_TYPE *);
	void OpenLiveViewStream();
	void OpenCameraOfIndex(int index);
	void OpenStream();
	void StreamCBFunc(J_tIMAGE_INFO *pAqImageInfo);
	bool OpenFactoryAndCamera();
	void CloseFactoryAndCamera();
};

}} //namespace brackets

#endif /*JAI_CAMERA_H*/
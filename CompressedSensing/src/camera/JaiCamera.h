#ifndef JAI_CAMERA_H
#define JAI_CAMERA_H

#include "src/camera/ICamera.h"
#include <string>
#include <stdint.h>
#include <Jai_Factory.h>
#include <functional>


class cv::Mat;

namespace CS {
namespace camera {

class JaiCamera : public ICamera {
public:
	JaiCamera(int, int);
	~JaiCamera();

	void grab();
	void stop();

	void registerCallback(std::function<void (const Frame& frame)>);
private:
	std::function<void (const Frame&)> callbackFunction;
	
	bool isCallbackRegistered;

	CAM_HANDLE camHandle;
	FACTORY_HANDLE factoryHandle;
	VIEW_HANDLE viewHandle;
	THRD_HANDLE hThread;

	int64_t getParameter(std::string paramName);
	int64_t getSizeOfBuffer();

	void validator(const char *, J_STATUS_TYPE *);
	void openCameraOfIndex(int index);
	void openStream();
	void streamCBFunc(J_tIMAGE_INFO *pAqImageInfo);
	void callbackWrapper(J_tIMAGE_INFO *pAqImageInfo);
	bool openFactoryAndCamera();
	void closeFactoryAndCamera();
};

}} //namespace brackets

#endif /*JAI_CAMERA_H*/
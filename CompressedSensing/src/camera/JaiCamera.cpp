#include "src/camera/JaiCamera.h"
#include "src/exceptions/Exceptions.h"

#include <string>
#include <conio.h>
#include <iomanip>
#include <memory>
#include <iostream>
#include <cmath>

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <cmath>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv/cv.h>

using namespace std;
using namespace cv;
using namespace CS::camera;
using namespace CS::exception;

JaiCamera::JaiCamera(int imageWidth, int imageHeight, double measurementRatio): 
	numberOfMeasurements(int(ceil(measurementRatio * (double)(imageWidth * imageHeight)))), currentRow(0), measurementComplete(false) {
	measurementMatrix = cv::Mat::zeros(numberOfMeasurements, imageWidth * imageHeight, CV_8UC1);
	BOOST_LOG_TRIVIAL(debug) <<"JaiCamera::JaiCamera: measurementMatrix allocated";
}
JaiCamera::~JaiCamera() {
	closeFactoryAndCamera();
}

Mat& JaiCamera::gatherMeasurements() {
	openFactoryAndCamera();
	openMeasurementStream();

	waitUntilMeasurementFinished();
	return measurementMatrix;
}

void JaiCamera::waitUntilMeasurementFinished() {
	while(!measurementComplete) {
		Sleep(100);
	}
}

void JaiCamera::openCameraOfIndex(int index) {
	int8_t m_sCameraId[J_CAMERA_ID_SIZE];
	uint32_t iSize = (uint32_t)sizeof(m_sCameraId);
	J_Factory_GetCameraIDByIndex(factoryHandle, index, m_sCameraId, &iSize);
	J_Camera_Open(factoryHandle, m_sCameraId, &camHandle);
}

void JaiCamera::validator(const char *name, J_STATUS_TYPE *retVal) {
	if ((*retVal) != J_ST_SUCCESS) {
		throw new JaiCameraException(name, *retVal);
	} 
}

bool JaiCamera::openFactoryAndCamera() {
	J_STATUS_TYPE retVal;
	bool8_t bHasChange;
	retVal = J_Factory_Open((const int8_t *)"", &factoryHandle);
	validator("J_Factory_Open", &retVal);
	retVal = J_Factory_UpdateCameraList(factoryHandle, &bHasChange);
	validator("J_Factory_UpdateCameraList", &retVal);

	openCameraOfIndex(0);
	return true;
}

int64_t JaiCamera::getParameter(string paramName) {
	int64_t answer;
	NODE_HANDLE hNode;
	J_Camera_GetNodeByName(camHandle, (int8_t *)(paramName.c_str()), &hNode);
	J_Node_GetValueInt64(hNode, false, &answer);
	return answer;
}

int64_t JaiCamera::getSizeOfBuffer() {
	const int64_t width = getParameter("Width");
	const int64_t height = getParameter("Height");
	const int64_t pixelFormat = getParameter("PixelFormat");
	const unsigned int bpp = J_BitsPerPixel(pixelFormat);

	return bpp * height * width / 8;
}

void JaiCamera::streamCBFunc(J_tIMAGE_INFO *pAqImageInfo) {
	IplImage *m_pImg = cvCreateImage(
		cvSize(pAqImageInfo->iSizeX, pAqImageInfo->iSizeY), IPL_DEPTH_8U, 1);
	IplImage *m_pOutputImg = cvCreateImage(cvGetSize(m_pImg), 8, 1);
	IplImage *m_pColorOutputImg = cvCreateImage(cvGetSize(m_pImg), 8, 3);

	memcpy(m_pImg->imageData, pAqImageInfo->pImageBuffer, m_pImg->imageSize);
	// cvSaveImage("foo_gray.bmp", m_pImg);

	cv::Mat input(m_pImg, true);
	cv::Mat output(m_pColorOutputImg);
	cv::cvtColor(input, output, CV_BayerRG2RGB);
	cv::namedWindow("camera-image");
	cv::imshow("camera-image", output);
}

void JaiCamera::getMeasurementMatrixCBFunc(J_tIMAGE_INFO *pAqImageInfo) {
	memcpy(measurementMatrix.data, pAqImageInfo->pImageBuffer, pAqImageInfo->iImageSize);
}

void JaiCamera::openStream() {
	long long int buffersize = getSizeOfBuffer();
	J_STATUS_TYPE retVal = J_Image_OpenStream(
		camHandle, 0, reinterpret_cast<J_IMG_CALLBACK_OBJECT>(this),
		reinterpret_cast<J_IMG_CALLBACK_FUNCTION>(&JaiCamera::streamCBFunc),
		&hThread, (uint32_t)buffersize);
	validator("J_Image_OpenStream", &retVal);
}

void JaiCamera::openMeasurementStream() {
	long long int buffersize = getSizeOfBuffer();
	J_STATUS_TYPE retVal = J_Image_OpenStream(
		camHandle, 0, reinterpret_cast<J_IMG_CALLBACK_OBJECT>(this),
		reinterpret_cast<J_IMG_CALLBACK_FUNCTION>(&JaiCamera::getMeasurementMatrixCBFunc),
		&hThread, (uint32_t)buffersize);
	validator("J_Image_OpenStream", &retVal);
}

void JaiCamera::openLiveViewStream() {
	J_STATUS_TYPE retVal;
	openStream();
	retVal = J_Camera_ExecuteCommand(camHandle, (int8_t *)"AcquisitionStart");
	validator("J_Camera_AcqStart", &retVal);
	Sleep(2000);
	J_Camera_ExecuteCommand(camHandle, (int8_t *)"AcquisitionStop");
	validator("J_Camera_AcqStop", &retVal);
	J_Image_CloseStream(hThread);
}

void JaiCamera::closeFactoryAndCamera() {
	if (camHandle) {
		J_Camera_Close(camHandle);
		camHandle = NULL;
	}

	if (factoryHandle) {
		J_Factory_Close(factoryHandle);
		factoryHandle = NULL;
	}
}
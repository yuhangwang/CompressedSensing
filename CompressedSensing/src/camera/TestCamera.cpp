#include "src/camera/TestCamera.h"
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <iostream>
#include <exception>

using namespace CS::camera;

TestCamera::TestCamera(int imageWidth, int imageHeight) : isGrabbing(false) {
	BOOST_LOG_TRIVIAL(debug) << "TestCamera construction\n";
	this->imageWidth = imageWidth;
	this->imageHeight = imageHeight;
	loadImage();
}
TestCamera::~TestCamera() {
	if(displayThread.joinable()) {displayThread.join();}
	if(processingThread.joinable()) {processingThread.join();}
	BOOST_LOG_TRIVIAL(debug) << "TestCamera de-construction\n";
}


void TestCamera::grab() {
	isGrabbing = true;
	displayThread = std::thread(&TestCamera::displayImage, this);
	processingThread = std::thread(&TestCamera::processImage, this);
}

void TestCamera::stop() {
	isGrabbing = false;
	displayThread.join();
	processingThread.join();
	BOOST_LOG_TRIVIAL(debug) << "TestCamera stopped gracefully\n";
}

void TestCamera::registerCallback(std::function<void (Frame& frame)> function) {
	processingFunction = function;
}

// private methods
void TestCamera::loadImage() {
	BOOST_LOG_TRIVIAL(debug) << "About to load pic in TestCamera\n";
	cv::Mat inputPic = cv::imread("../pics/rihanna.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	if(inputPic.data) {
		BOOST_LOG_TRIVIAL(debug) << "inputPic loaded\n";
	}else {
		throw new std::runtime_error("Could not load test camera picture");
	}
	internalPic = cv::Mat(imageHeight, imageWidth, CV_8UC1);
	BOOST_LOG_TRIVIAL(debug) <<"input type = "<<inputPic.type() << "and internal = "<<internalPic.type()<<std::endl;
	cv::resize(inputPic, internalPic, cv::Size(imageWidth, imageHeight));
	BOOST_LOG_TRIVIAL(debug) << "Pic resized\n";
}

void TestCamera::displayImage() {
	cv::namedWindow("display", CV_WINDOW_AUTOSIZE);
	while(isGrabbing) {
		std::this_thread::sleep_for(std::chrono::milliseconds(30));
		m.lock();
		cv::imshow("display", internalPic);
		m.unlock();

		cv::waitKey(5);
		m.lock();
		loadImage();
		m.unlock();
		isNewDataReady = true;
	}
	BOOST_LOG_TRIVIAL(debug) << "TestCamera::displayImage finished";
}

void TestCamera::processImage() {
	while(isGrabbing) {
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		if(isNewDataReady) {
			m.lock();
			Frame frame = matToFrame(internalPic);
			m.unlock();

			//simulate callback which has to sync itself with acquiring thread
			processingFunction(frame);
			isNewDataReady = false;
		}
	}

	BOOST_LOG_TRIVIAL(debug) << "TestCamera::processImage finished";
}

Frame TestCamera::matToFrame(cv::Mat& image) {
	long simulatedTimestamp = boost::posix_time::microsec_clock::local_time().time_of_day().total_milliseconds();
	return Frame(simulatedTimestamp, image.size().width, image.size().height, image.data);
}
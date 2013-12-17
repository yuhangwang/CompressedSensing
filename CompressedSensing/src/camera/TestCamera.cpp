#include "src/camera/TestCamera.h"
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <iostream>

using namespace CS::camera;

TestCamera::TestCamera(int imageWidth, int imageHeight) {BOOST_LOG_TRIVIAL(info) << "TestCamera construction\n";}
TestCamera::~TestCamera() {BOOST_LOG_TRIVIAL(info) << "TestCamera de-construction\n";}

void TestCamera::grab() {
}

void TestCamera::stop() {
}

void TestCamera::registerCallback(std::function<void (Frame& frame)> function) {
}

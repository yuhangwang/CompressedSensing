#pragma once

#include <opencv2/opencv.hpp>

namespace CS {
namespace algorithm {

class ICSAlgorithm {
public:
	ICSAlgorithm();
	virtual ~ICSAlgorithm()=0;
	virtual cv::Mat recoverImage(const cv::Mat& measurementMatrix, const cv::Mat& observations, const cv::Mat& startingSolution)=0;
};




}}
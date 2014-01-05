#pragma once

#include <opencv2/core/core.hpp>

namespace CS {
namespace solver {

class ISolver {
public:
	virtual void createBinaryMeasurementMatrix(int rows, int cols, cv::Mat*)=0;
	virtual cv::Mat product(const cv::Mat& A, const cv::Mat& y)=0;
	virtual cv::Mat transProduct(const cv::Mat& A, const cv::Mat& y)=0;
	virtual cv::Mat linsolve(const cv::Mat& A,const cv::Mat& y)=0;
	virtual ~ISolver()=0;
};


}}
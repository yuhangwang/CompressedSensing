#pragma once
#include "src/algorithm/ICSAlgorithm.h"

namespace CS {
namespace algorithm {

class TvqcAlgorithm : public ICSAlgorithm {
public:
	TvqcAlgorithm();
	~TvqcAlgorithm();
	cv::Mat recoverImage(const cv::Mat& measurementMatrix, const cv::Mat& observations, const cv::Mat& startingSolution);
};


}}//namespace end
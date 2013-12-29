#include "src/algorithm/TvqcAlgorithm.h"

using namespace CS::algorithm;

TvqcAlgorithm::TvqcAlgorithm() {}
TvqcAlgorithm::~TvqcAlgorithm() {}

cv::Mat TvqcAlgorithm::recoverImage(const cv::Mat& measurementMatrix, const cv::Mat& observations, const cv::Mat& startingSolution) {
	cv::Mat outputImage = startingSolution.clone();

	return outputImage;
}
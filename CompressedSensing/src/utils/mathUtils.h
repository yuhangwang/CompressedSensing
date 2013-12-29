#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include <opencv2/opencv.hpp>
#include <vector>

namespace CS {
namespace math {

	class MathUtils {
	public:
		static void randperm(int n, int* vec);
		static std::vector<std::vector<float>> matToStdMatrix(const cv::Mat& A);
		static void normalizeImage(cv::Mat& input);
	};

}} //namespace end

#endif /*MATH_UTILS_H*/
#include "src/utils/mathUtils.h"
#include "src/utils/log.h"
#include <cstdlib>

void CS::math::MathUtils::randperm(int n, int* perm) {
	int i, j, t;

	for(i=0; i<n; i++)
		perm[i] = i;
	for(i=0; i<n; i++) {
		j = rand()%(n-i)+i;
		t = perm[j];
		perm[j] = perm[i];
		perm[i] = t;
	}
}

std::vector<std::vector<float>> CS::math::MathUtils::matToStdMatrix(const cv::Mat& A) {
	int height = A.size().height;
	int width = A.size().width;
	std::vector<std::vector<float>> stdMatrix(height);
	LOG_DEBUG("size of mat = ("<<A.size().height<<","<<A.size().width<<")");
	for(int i = 0; i < height; i++) {
		stdMatrix[i].resize(width);
		std::copy(A.data + width * i, A.data + width * i + height, stdMatrix[i].begin());
	}

	return stdMatrix;
}

void CS::math::MathUtils::normalizeImage(cv::Mat& input) {
	input -= cv::norm(input, cv::NORM_L2);
	input -= cv::mean(input);
}


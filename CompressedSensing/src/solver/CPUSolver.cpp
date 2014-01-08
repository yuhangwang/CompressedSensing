#include "CPUSolver.h"
#include "Eigen/Core"
#include "Eigen/LU"
#include "Eigen/QR"
#include "Eigen/Dense"
#include <opencv2/core/eigen.hpp>

#include <algorithm>
#include <functional>

using namespace CS::solver::cpu;
using namespace Eigen;

CPUSolver::CPUSolver()
{
}

CPUSolver::~CPUSolver() {}

void CPUSolver::createBinaryMeasurementMatrix(int rows, int cols, cv::Mat *output) {

	float* outputData = (float*)output->data;
	Eigen::Map<Matrix<float, Dynamic, Dynamic, RowMajor>> eigenM(outputData, output->rows, output->cols);
	eigenM.setRandom();
	auto f = std::bind(&CPUSolver::setOneZero, this, std::placeholders::_1);
	std::transform(eigenM.data(), eigenM.data() + eigenM.size(), eigenM.data(), f);
}

cv::Mat CPUSolver::product(const cv::Mat& A, const cv::Mat& b) {
	return A * b;
}

cv::Mat CPUSolver::transProduct(const cv::Mat& A, const cv::Mat& b) {
	cv::Mat At = cv::Mat(A.cols, A.rows, A.type());
	cv::transpose(A, At);
	return At * b;
}

cv::Mat CPUSolver::linsolve(const cv::Mat& A, const cv::Mat& b) {
	cv::Mat output(A.cols, 1, A.type());
	float *A_data = (float*)A.data;
	float *b_data = (float*)b.data;
	Eigen::Map<Matrix<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>> eigenMatrix(A_data, A.rows, A.cols);
	Eigen::Map<Matrix<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>> eigenVector(b_data, b.rows, b.cols);
	Eigen::MatrixXf x;
	if(A.rows == A.cols) {
		x = eigenMatrix.lu().solve(eigenVector);		
	}else {
	    x = eigenMatrix.fullPivHouseholderQr().solve(eigenVector);
	}
	cv::eigen2cv(x, output);
	return output;
}

float CPUSolver::setOneZero(float number) {
    return number > 0.0f ? 1.0f : 0.0f;
}

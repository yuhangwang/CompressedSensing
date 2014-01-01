#include "src/gpu/GPUSolver.h"
#include "src/utils/log.h"
#include "src/utils/mathUtils.h"

#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/triangular.hpp>
#include <boost/numeric/ublas/matrix_sparse.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/vector_proxy.hpp>
#include <boost/numeric/ublas/lu.hpp>
#include <boost/numeric/ublas/io.hpp>

#include <iostream>
#include <string>
#include <sstream>
#include <random>
#include <vector>

using namespace cv;
using namespace CS::gpu;
using namespace CS::math;

// public methods

void GPUSolver::createBinaryMeasurementMatrix(int rows, int cols, cv::Mat* measurementMatrix) {

	GenerationParameters generationParameters = getGenerationParameters(rows, cols);
	performMatrixGeneration(generationParameters, measurementMatrix);
}

cv::Mat GPUSolver::product(const cv::Mat& A, const cv::Mat& y) {
	cv::Mat x = cv::Mat(A.size().height, 1, A.type());

	viennacl::matrix<float> gpuMatrix(A.size().height, A.size().width);
	viennacl::vector<float> gpuVector(A.size().width);
	viennacl::vector<float> gpuResult(A.size().height);

	StdMatrix stdMatrix = CS::math::MathUtils::matToStdMatrix(A);
	
	//copy to GPU
	viennacl::copy(stdMatrix, gpuMatrix);
	viennacl::copy(y.begin<float>(), y.end<float>(), gpuVector.begin());

	gpuResult = viennacl::linalg::prod(gpuMatrix, gpuVector);

	//copy back to host
	viennacl::copy(gpuResult.begin(), gpuResult.end(), x.begin<float>());

	return x;
}



cv::Mat GPUSolver::transProduct(const cv::Mat& A, const cv::Mat& y) {
	cv::Mat x = cv::Mat(A.size().width, 1, A.type());
	LOG_DEBUG("return vector size = ("<<x.size().height<<","<<x.size().width<<")");
	LOG_DEBUG("y size = ("<<y.size().height<<","<<y.size().width<<")");
	viennacl::matrix<float> gpuMatrix(A.size().height, A.size().width);
	viennacl::vector<float> gpuVector(y.size().height);
	viennacl::vector<float> gpuResult(A.size().width);

	StdMatrix stdMatrix = CS::math::MathUtils::matToStdMatrix(A);
	
	//copy to GPU
	viennacl::copy(stdMatrix, gpuMatrix);
	viennacl::copy(y.begin<float>(), y.end<float>(), gpuVector.begin());

	gpuResult = viennacl::linalg::prod(trans(gpuMatrix), gpuVector);

	//copy back to host
	cv::MatIterator_<float> it = x.begin<float>();
	viennacl::copy(gpuResult.begin(), gpuResult.end(), it);

	return x;
}

cv::Mat GPUSolver::linsolve(const cv::Mat& A, const cv::Mat& y) {
	cv::Mat x;

	if(isMatrixSquare(A)) {
		x = LUSolve(A, y);
	}else {
		x = QRMinEnergySolve(A,y);
	}

	return x;
}

bool GPUSolver::isMatrixSquare(const cv::Mat& A) {
	return (A.size().height == A.size().width);
}

// private methods

cv::Mat GPUSolver::LUSolve(const cv::Mat& A, const cv::Mat& y) {
	cv::Mat x;
	std::vector<std::vector<float>> stdMatrix = MathUtils::matToStdMatrix(A);
	viennacl::matrix<float> gpuMatrix(A.size().height, A.size().width);
	viennacl::vector<float> gpuVector(y.size().height);

	//copy from host to GPU
	viennacl::copy(stdMatrix, gpuMatrix);
	viennacl::copy(y.begin<float>(),y.end<float>(),gpuVector.begin());

	viennacl::linalg::lu_factorize(gpuMatrix);
	viennacl::linalg::lu_substitute(gpuMatrix, gpuVector);

	x = cv::Mat(y.size().height, 1, y.type());

	viennacl::copy(gpuVector.begin(), gpuVector.end(), x.begin<float>());
	LOG_DEBUG("x = "<<x);
	return x;
}

cv::Mat GPUSolver::QRMinEnergySolve(const cv::Mat& A, const cv::Mat& y) {
	cv::Mat x;
	
	typedef boost::numeric::ublas::matrix<float> BoostMatrix;
	typedef std::vector<std::vector<float>> StdMatrix;
	int rows = A.size().height;
	int columns = A.size().width;

	BoostMatrix boostMatrix = MathUtils::matToBoostMatrix(A);
	BoostMatrix Q(rows, rows);
	BoostMatrix R(rows, columns);
	viennacl::matrix<float> gpuMatrix;

	//copy data to GPU
	viennacl::copy(boostMatrix, gpuMatrix);

	//fill GPU matrix with Householder vectors. Q, R not stored (implicit)
	std::vector<float> betas = viennacl::linalg::inplace_qr(gpuMatrix);

	//copy back to CPU
	viennacl::copy(gpuMatrix, boostMatrix);

	//create explicit Q,R matrices
	viennacl::linalg::recoverQ(boostMatrix, betas, Q, R);

	LOG_DEBUG("Q = "<<Q<<" R = "<<R);

	return x;
}

void GPUSolver::performMatrixGeneration(GenerationParameters& parameters, cv::Mat* output) {
	viennacl::vector<float> randomMatrix(std::get<1>(parameters));
	viennacl::vector<int> seeds(std::get<0>(parameters));
	LOG_DEBUG("Created viennacl randomMatrix of size "<<randomMatrix.size()<<" and seed vector of size "<<seeds.size());

	initializeSeeds(parameters, &seeds);
	runRandomGeneratorKernel(parameters, seeds, &randomMatrix);

	cv::MatIterator_<float> it = output->begin<float>();
	viennacl::copy(randomMatrix.begin(), randomMatrix.end(), it);
}

void GPUSolver::runRandomGeneratorKernel(GenerationParameters& parameters, viennacl::vector<int>& seeds, viennacl::vector<float>* randomMatrix) {
	std::string randomGeneratorProgram = getRandKernelSource();
	viennacl::ocl::program & randomProgram = viennacl::ocl::current_context().add_program(randomGeneratorProgram, "randomGeneratorProgram");
	viennacl::ocl::kernel & randomKernel = randomProgram.get_kernel("generateRandomMatrix");
	randomKernel.global_work_size(0,std::get<0>(parameters));

	viennacl::ocl::enqueue(randomKernel(seeds, *randomMatrix, static_cast<cl_uint>(std::get<2>(parameters)), static_cast<cl_uint>(std::get<1>(parameters))));
	LOG_DEBUG("generateRandomMatrix kernel enqueued");
}

void GPUSolver::initializeSeeds(GenerationParameters& parameters, viennacl::vector<int>* seedVector) {
	std::vector<int> seedsInitializer(std::get<0>(parameters));

	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(0, 2147483647);
	for(unsigned int i=0; i < seedsInitializer.size(); i++) {
		seedsInitializer[i] = distribution(generator);
	}

	viennacl::fast_copy(seedsInitializer, *seedVector);
}

GenerationParameters GPUSolver::getGenerationParameters(int rows, int cols) {
	int threadsTotal = 128;
	int matrixSize = rows * cols;
	int nPerWorkItem = (int)ceil((double)matrixSize/(double)threadsTotal);
	LOG_DEBUG("Random matrix generation parameters: threads_total = "<<threadsTotal<<" matrixSize = "<<matrixSize<<" items per work item = "<<nPerWorkItem);
	return std::make_tuple(threadsTotal, matrixSize, nPerWorkItem);
}

std::string GPUSolver::getRandKernelSource() {
	std::ostringstream output("");

	output << 
	"__kernel void generateRandomMatrix(__global int * seedMemory, __global float * randomMatrix, unsigned int nPerWorkItem, unsigned int matrixSize) \n"
	"{ \n"
	"	int global_id = get_global_id(0);\n"
	"	float seed = seedMemory[global_id];\n"
	"	float output = 0.0;\n"
	"	for(unsigned int i = 0; i < nPerWorkItem; i++) { \n"
	"		if((global_id * nPerWorkItem + i) < matrixSize) { \n"
	"			seed = fmod((seed * 16807), 2147483647); \n" //2^31-1
	"			output = (seed < 1073741823.5) ? 0.0 : 1.0; \n" // 0.5 * 2^31
	"			randomMatrix[global_id * nPerWorkItem + i]	= output ; \n"
	"		}\n"
	"	}\n"
	"};\n"
	;

	return output.str();
}

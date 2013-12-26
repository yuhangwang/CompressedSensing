#include "src/gpu/GPUSolver.h"
#include "src/utils/log.h"

#include <iostream>
#include <string>
#include <sstream>
#include <random>

using namespace cv;
using namespace CS::gpu;

// public methods

void GPUSolver::createBinaryMeasurementMatrix(int rows, int cols, cv::Mat* measurementMatrix) {

	GenerationParameters generationParameters = getGenerationParameters(rows, cols);
	performMatrixGeneration(generationParameters, measurementMatrix);
}

// private methods

void GPUSolver::performMatrixGeneration(GenerationParameters& parameters, cv::Mat* output) {
	viennacl::vector<uchar> randomMatrix(std::get<1>(parameters));
	viennacl::vector<int> seeds(std::get<0>(parameters));
	LOG_DEBUG("Created viennacl randomMatrix of size "<<randomMatrix.size()<<" and seed vector of size "<<seeds.size());

	initializeSeeds(parameters, &seeds);
	runRandomGeneratorKernel(parameters, seeds, &randomMatrix);

	cv::MatIterator_<uchar> it = output->begin<uchar>();
	viennacl::copy(randomMatrix.begin(), randomMatrix.end(), it);
}

void GPUSolver::runRandomGeneratorKernel(GenerationParameters& parameters, viennacl::vector<int>& seeds, viennacl::vector<uchar>* randomMatrix) {
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
	"__kernel void generateRandomMatrix(__global int * seedMemory, __global uchar * randomMatrix, unsigned int nPerWorkItem, unsigned int matrixSize) \n"
	"{ \n"
	"	int global_id = get_global_id(0);\n"
	"	double seed = seedMemory[global_id];\n"
	"	char output = 0;\n"
	"	for(unsigned int i = 0; i < nPerWorkItem; i++) { \n"
	"		if((global_id * nPerWorkItem + i) < matrixSize) { \n"
	"			seed = fmod((seed * 16807), 2147483647); \n" //2^31-1
	"			output = (seed < 1073741823) ? 0 : 1; \n" // 0.5 * 2^31
	"			randomMatrix[global_id * nPerWorkItem + i]	= output ; \n"
	"		}\n"
	"	}\n"
	"};\n"
	;

	return output.str();
}

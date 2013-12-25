#ifndef GPU_MANAGER_H
#define GPU_MANAGER_H
#define VIENNACL_WITH_OPENCL
//#define VIENNACL_DEBUG_ALL

#include <memory>
#include <tuple>
#include <opencv2/core/core.hpp>

#include "viennacl/ocl/backend.hpp"
#include "viennacl/vector.hpp"

namespace CS {
namespace gpu {

typedef std::tuple<int, int, int> GenerationParameters;

class GPUSolver {
public:
	void createBinaryMeasurementMatrix(int rows, int cols, cv::Mat*);

private:
	//methods
	std::string getRandKernelSource();
	GenerationParameters getGenerationParameters(int, int);
	void performMatrixGeneration(GenerationParameters&, cv::Mat* output);
	void initializeSeeds(GenerationParameters&, viennacl::vector<int>* seedVector);
	void runRandomGeneratorKernel(GenerationParameters&, viennacl::vector<int>& seeds, viennacl::vector<uchar>* randomMatrix);

	//data
};


}} // namespace end

#endif /*GPU_MANAGER_H*/
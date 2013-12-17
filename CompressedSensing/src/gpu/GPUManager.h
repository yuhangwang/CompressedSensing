#ifndef GPU_MANAGER_H
#define GPU_MANAGER_H

#include "viennacl/ocl/device.hpp"
#include "viennacl/ocl/platform.hpp"
#include "viennacl/scalar.hpp"
#include "viennacl/vector.hpp"
#include "viennacl/matrix.hpp"
#include "viennacl/linalg/prod.hpp"
#include "viennacl/matrix_proxy.hpp"
#include "viennacl/linalg/lu.hpp"

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

#include <opencv2/core/core.hpp>

namespace CS {
namespace gpu {

class GPUManager {
public:
	cv::Mat& createBinaryMatrix(int rows, int cols);

private:
	cv::Mat measurementMatrix;
};


}} // namespace end

#endif /*GPU_MANAGER_H*/
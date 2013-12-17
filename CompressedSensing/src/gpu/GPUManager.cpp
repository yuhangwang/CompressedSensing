#include "src/gpu/GPUManager.h"

using namespace cv;
using namespace CS::gpu;

Mat& GPUManager::createBinaryMatrix(int rows, int cols) {
	return measurementMatrix;
}


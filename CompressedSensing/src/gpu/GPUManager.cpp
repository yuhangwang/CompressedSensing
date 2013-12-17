#include "src/gpu/GPUManager.h"

using namespace cv;
using namespace CS::gpu;

Mat& GPUManager::createBinaryMeasurementMatrix(int rows, int cols) {
	return measurementMatrix;
}


#ifndef EXPERIMENT_HANDLER_H
#define EXPERIMENT_HANDLER_H

#include <memory>
#include "src/camera/ICamera.h"

namespace CS {
namespace experiment {

struct ExperimentParameters {
public:
	int imageWidth;
	int imageHeight;
	double measurementRatio;
};

class ExperimentHandler {
public:
	ExperimentHandler(std::shared_ptr<camera::ICamera> camera, ExperimentParameters& params);
private:
	void simpleTransform(camera::Frame& frame);
};

}} //namespace end

#endif /*EXPERIMENT_HANDLER_H*/
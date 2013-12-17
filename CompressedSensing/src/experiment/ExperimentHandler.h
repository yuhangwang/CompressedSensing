#ifndef EXPERIMENT_HANDLER_H
#define EXPERIMENT_HANDLER_H

#include <memory>
#include "src/camera/ICamera.h"

namespace CS {
namespace experiment {

struct ExperimentParameters {
public:
	double measurementRatio;
};

class ExperimentHandler {
public:
	ExperimentHandler(std::shared_ptr<camera::ICamera> camera, int imageWidth, int imageHeight, ExperimentParameters& params);

};

}} //namespace end

#endif /*EXPERIMENT_HANDLER_H*/
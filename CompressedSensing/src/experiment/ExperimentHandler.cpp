#include "src/experiment/ExperimentHandler.h"

using namespace CS::experiment;
using namespace CS::camera;


ExperimentHandler::ExperimentHandler(std::shared_ptr<ICamera> camera, ExperimentParameters& params) {
	camera->registerCallback(std::bind(&ExperimentHandler::simpleTransform, this, std::placeholders::_1));
}

void ExperimentHandler::simpleTransform(Frame& frame) {
}
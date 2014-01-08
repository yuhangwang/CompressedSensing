#define __CL_ENABLE_EXCEPTIONS
#define OPENCV_VERSION 244

#include <thread>
#include <memory>
#include <iostream>
#include <CL/cl.hpp>
#include <boost/program_options.hpp>
#include <boost/timer/timer.hpp>

#include "src/utils/log.h"
#include "src/utils/utils.h"
#include "src/exceptions/Exceptions.h"

#include "src/camera/CameraFactory.h"
#include "src/algorithm/AlgorithmFactory.h"
#include "src/solver/SolverFactory.h"

#include "src/experiment/ExperimentHandler.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace CS;
using namespace CS::camera;
using namespace CS::algorithm;
using namespace CS::experiment;
using namespace CS::solver;
namespace opts = boost::program_options;

#ifdef _DEBUG
#pragma comment (lib, "opencv_highgui244d.lib")
#pragma comment (lib, "opencv_core244d.lib")
#pragma comment (lib, "opencv_imgproc244d.lib")
#else
#pragma comment (lib, "opencv_highgui244.lib")
#pragma comment (lib, "opencv_core244.lib")
#pragma comment (lib, "opencv_imgproc244d.lib")
#endif

int main(int argc, char **argv) {
	double measurementRatio = 0.0;
	bool verbose = false;
	string cameraName, algorithmName, solverType;

	try {
		LOG_INFO("Application start");
		LOG_DEBUG("main_thread_hash = "<<(int)std::this_thread::get_id().hash());
		opts::options_description desc("Allowed options");
        opts::variables_map vm = utils::setAndRunCommandLineArgs(argc, argv, &measurementRatio, &verbose, &cameraName, &algorithmName, &solverType, desc);

		if(vm.count("help")) {
			cout << desc << "\n";
			return 0;
		}

		utils::setLoggingParameters(verbose);

		int imageWidth = vm["image-width"].as<int>();
		int imageHeight = vm["image-height"].as<int>();

		utils::validateInputArguments(imageWidth, imageHeight, measurementRatio);
		utils::makeMeasurementNumberDivisibleBy16(imageHeight * imageWidth, &measurementRatio);//ViennaCL::qr condition
		std::shared_ptr<ICamera> pCamera(CameraFactory::getInstance(cameraName, imageWidth, imageHeight));
		std::shared_ptr<ICSAlgorithm> pAlgorithm(AlgorithmFactory::getInstance(algorithmName));
		std::shared_ptr<ISolver> pSolver(SolverFactory::getInstance(solverType));
		ExperimentParameters params(measurementRatio, imageWidth, imageHeight);

		ExperimentHandler handler(pCamera, pAlgorithm, pSolver, params);
		handler.handleExperiment();

		LOG_INFO("Application successful exit");
	}catch(std::bad_alloc& e) {
		cerr << "Probably not enough memory: "<<e.what() << std::endl;
	}catch(CS::exception::JaiCameraException& e) {
		cerr << "Exception in JaiCamera: " << e.what() << std::endl;
	}catch(CS::exception::UnknownTypeException& e) {
		cerr << "type_error: "<<e.what() << std::endl;
	}catch(std::range_error& e) {
		cerr << "range_error: "<<e.what() << std::endl;
	}catch(std::runtime_error& e) {
		cerr << "runtime_error: "<<e.what() << std::endl;
	}catch(std::exception& e) {
		cerr << "error: "<<e.what() << std::endl;
	}catch(...) {
		cerr << "Exception of unknown type!\n";
	}

	return 0;
}

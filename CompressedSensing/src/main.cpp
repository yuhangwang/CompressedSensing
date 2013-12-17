#define __CL_ENABLE_EXCEPTIONS

#include <memory>
#include <iostream>
#include <CL/cl.hpp>
#include <boost/program_options.hpp>
#include <boost/timer/timer.hpp>
#include "utils/utils.h"
#include "src/exceptions/Exceptions.h"
#include "src/camera/CameraFactory.h"
#include "src/gpu/GPUManager.h"
#include "src/experiment/ExperimentHandler.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace CS;
using namespace CS::camera;
using namespace CS::experiment;
namespace opts = boost::program_options;
using namespace boost::numeric::ublas;

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
	string cameraName;

	try {
		BOOST_LOG_TRIVIAL(info) << "Application start\n";
		opts::options_description desc("Allowed options");
		opts::variables_map vm = utils::setAndRunCommandLineArgs(argc, argv, &measurementRatio, &verbose, &cameraName, desc);

		if(vm.count("help")) {
			cout << desc << "\n";
			return 0;
		}

		utils::setLoggingParameters(verbose);

		int imageWidth = vm["image-width"].as<int>();
		int imageHeight = vm["image-height"].as<int>();

		utils::validateInputArguments(imageWidth, imageHeight, measurementRatio);
		std::shared_ptr<ICamera> pCamera(CameraFactory::getInstance(cameraName, imageWidth, imageHeight));

		ExperimentParameters params;
		params.measurementRatio = measurementRatio;
		params.imageWidth = imageWidth;
		params.imageHeight = imageHeight;

		ExperimentHandler handler(pCamera, params);

		cout << viennacl::ocl::device().info();

		BOOST_LOG_TRIVIAL(info) << "Application successful exit";

	}catch(std::bad_alloc& e) {
		cerr << "Probably not enough memory: "<<e.what() << "\n";
	}catch(CS::exception::JaiCameraException& e) {
		cerr << "Exception in JaiCamera: " << e.what() << "\n";
	}catch(CS::exception::UnknownTypeException& e) {
		cerr << "type_error: "<<e.what() << "\n";
	}catch(range_error& e) {
		cerr << "range_error: "<<e.what() << "\n";
	}catch(std::exception& e) {
		cerr << "error: "<<e.what() << "\n";
	}catch(...) {
		cerr << "Exception of unknown type!\n";
	}

	
	return 0;
}
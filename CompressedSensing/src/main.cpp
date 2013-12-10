#define __CL_ENABLE_EXCEPTIONS

#include <memory>
#include <iostream>
#include <CL/cl.hpp>
#include <boost/program_options.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include "utils/utils.h"
#include "src/exceptions/Exceptions.h"
#include "src/camera/CameraFactory.h"

using namespace std;
using namespace CS;
using namespace CS::camera;
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

ostringstream CS::exception::UnknownTypeException::msg;

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
		std::shared_ptr<ICamera> pCamera(CameraFactory::getInstance(cameraName, imageWidth, imageHeight, measurementRatio));


	}catch(CS::exception::UnknownTypeException& e) {
		cerr << "type_error: "<<e.what() << "\n";
	}catch(range_error& e) {
		cerr << "range_error: "<<e.what() << "\n";
	}catch(std::exception& e) {
		cerr << "error: "<<e.what() << "\n";
	}catch(...) {
		cerr << "Exception of unknown type!\n";
	}

	BOOST_LOG_TRIVIAL(info) << "Application successful exit";
	return 0;
}
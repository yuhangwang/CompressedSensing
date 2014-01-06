#pragma once

#include <opencv2/core/core.hpp>
#include <boost/numeric/ublas/matrix.hpp>

using namespace boost::numeric::ublas;

namespace CS {
namespace test {

class TestUtils {
public:
	static void fillMatByIndex(cv::Mat& A);

	static double maxRelativeError(matrix<double> const& m1, matrix<double> const&m2);
};

}}
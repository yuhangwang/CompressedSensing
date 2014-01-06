#include "test/testutils/TestUtils.h"

using namespace CS::test;

void TestUtils::fillMatByIndex(cv::Mat& A) {
    for(int i = 0; i < A.rows; i++) {
        for(int j = 0; j < A.cols; j++) {
            A.at<float>(i,j) = (float)(i * A.rows + j + 1);
        }
    }
}

double TestUtils::maxRelativeError(boost::numeric::ublas::matrix<double> const &m1, boost::numeric::ublas::matrix<double> const &m2) {
	bool do_break = false;
	double max_error = 0;
	for (std::size_t i=0; i<m2.size1(); ++i)
	{
		for (std::size_t j=0; j<m2.size2(); ++j)
		{
			if (m1(i,j) != 0.0 && m2(i,j) != 0.0)
			{
				double rel_err = fabs(m1(i,j) - m2(i,j)) / fabs(m2(i,j) );

				if (rel_err > max_error)
					max_error = rel_err;
			}

			if (m1(i,j) != m1(i,j))
			{
				std::cout << "!!!" << std::endl;
				std::cout << "!!! NaN detected at i=" << i << " and j=" << j << std::endl;
				std::cout << "!!!" << std::endl;
				do_break = true;
				break;
			}
		}
		if (do_break)
			break;
	}
	return max_error;
}
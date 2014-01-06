#include "gtest/gtest.h"
#include "src/utils/mathUtils.h"
#include "test/testutils/TestUtils.h"

#include "boost/numeric/ublas/matrix.hpp"

using namespace CS::math;
using namespace CS::test;

TEST(MathUtilsTest, MatToStdMatrixTest) {
    cv::Mat A(3,3, CV_32FC1);
    TestUtils::fillMatByIndex(A);

    std::vector<std::vector<float>> stdMatrix = MathUtils::matToStdMatrix(A);

    for(int i = 0; i < stdMatrix.size(); i++) {
        for(int j = 0; j < stdMatrix[i].size(); j++) {
            EXPECT_EQ(stdMatrix[i][j], A.at<float>(i,j));
        }
    }
}

TEST(MathUtilsTest, MatToBoostMatrixTest) {
	cv::Mat A(3,3, CV_32FC1);
	TestUtils::fillMatByIndex(A);

	boost::numeric::ublas::matrix<float> mat = MathUtils::matToBoostMatrix(A);

	for(int i = 0; i < A.rows; i++) {
		for(int j = 0; j < A.cols; j++) {
			EXPECT_EQ(mat(i,j), A.at<float>(i,j));
		}
	}
	
}



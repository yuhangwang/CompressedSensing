#include "gtest/gtest.h"
#include "src/utils/mathUtils.h"
#include "test/testutils/TestUtils.h"

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



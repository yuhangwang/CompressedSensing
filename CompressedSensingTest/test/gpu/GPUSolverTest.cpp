#include "gtest/gtest.h"
#include "test/testutils/TestUtils.h"
#include "src/solver/ISolver.h"
#include "src/solver/GPUSolver.h"

using namespace CS::solver;
using namespace CS::solver::gpu;
using namespace CS::test;

class GPUSolverTest : public ::testing::Test {

protected:
	GPUSolverTest():solver(new GPUSolver()) {
	}
	virtual ~GPUSolverTest() {}

	virtual void SetUp() {
		solver.reset();
		solver = std::shared_ptr<ISolver>(new GPUSolver());
	}

	virtual void TearDown() {
	}

	//data
	std::shared_ptr<ISolver> solver;
};

TEST_F(GPUSolverTest, SolverConstructs) {
	std::shared_ptr<ISolver>gpuSolver(new GPUSolver());
	EXPECT_EQ(1,1);
}

TEST_F(GPUSolverTest, CreateRandomBinaryMatrixSuccessful) {
    cv::Mat randomMatrix(50, 50, CV_32FC1);
    solver->createBinaryMeasurementMatrix(randomMatrix.rows,randomMatrix.cols, &randomMatrix);

    cv::Scalar meanValue = cv::mean(randomMatrix);
	double _mean = meanValue[0];
    double max, min;
    cv::minMaxLoc(randomMatrix, &min, &max);
    //mean value close to 0.5
	EXPECT_LT(abs(_mean - 0.5), 0.05);
    EXPECT_EQ(max, 1.0);
    EXPECT_EQ(min, 0.0);
}

TEST_F(GPUSolverTest, MatrixVectorProductTest) {
    cv::Mat A(3,3, CV_32FC1);
    TestUtils::fillMatByIndex(A);
    cv::Mat b(3, 1, CV_32FC1);
    b.at<float>(0,0) = 1.0;
    b.at<float>(1,0) = 2.0;
    b.at<float>(2,0) = 3.0;

    cv::Mat c = A*b;

    cv::Mat output = solver->product(A,b);
    EXPECT_EQ(c.at<float>(0,0), output.at<float>(0,0));
    EXPECT_EQ(c.at<float>(1,0), output.at<float>(1,0));
    EXPECT_EQ(c.at<float>(2,0), output.at<float>(2,0));
}

TEST_F(GPUSolverTest, MinEnergyLinSolveTest) {
    cv::Mat A(16,32, CV_32FC1);
    TestUtils::fillMatByIndex(A);
    A.diag() *= 5.0;
    cv::Mat y(2,1, CV_32FC1);
    y.at<float>(0,0) = 5.0f;
    y.at<float>(1,0) = 10.0f;

    std::cout << "A = " <<A << "y = "<<y;
    cv::Mat x = solver->linsolve(A, y);

    EXPECT_EQ(x.rows, 3);
    EXPECT_EQ(x.cols, 1);
}




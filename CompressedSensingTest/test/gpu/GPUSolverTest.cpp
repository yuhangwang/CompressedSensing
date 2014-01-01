#include "gtest/gtest.h"
#include "src/gpu/GPUSolver.h"

using namespace CS::gpu;

class GPUSolverTest : public ::testing::Test {

protected:
	GPUSolverTest() {}
	virtual ~GPUSolverTest() {}

	virtual void SetUp() {
	}

	virtual void TearDown() {
	}

	//data
	GPUSolver solver;
};

TEST_F(GPUSolverTest, SolverConstructs) {
	GPUSolver solver;
	EXPECT_EQ(1,1);
}


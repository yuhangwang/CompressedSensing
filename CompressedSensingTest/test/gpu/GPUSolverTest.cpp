#include "gtest/gtest.h"
#include "src/solver/ISolver.h"
#include "src/solver/GPUSolver.h"

using namespace CS::solver;
using namespace CS::solver::gpu;

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


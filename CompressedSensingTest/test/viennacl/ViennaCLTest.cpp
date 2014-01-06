#define VIENNACL_WITH_UBLAS
#define VIENNACL_WITH_OPENCL

#include "gtest/gtest.h"
#include "test/testutils/TestUtils.h"

#include "viennacl/linalg/qr.hpp"

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>


typedef boost::numeric::ublas::matrix<float> BMatrix;

namespace CS {
namespace test {

void testFillMatrices(BMatrix& m, BMatrix& Q, BMatrix& R) {
	int rows = m.size1();
	int cols = m.size2();
	for (std::size_t i=0; i<rows; ++i) {
		for (std::size_t j=0; j<cols; ++j) {
			m(i,j) = -1.0 + (i+1)*(j+1)
							+ ( (rand() % 1000) - 500.0) / 1000.0;

			if (i == j)
			m(i,j) += 10.0;

			R(i,j) = 0.0;
		}

		for (std::size_t j=0; j<rows; ++j)
			Q(i,j) = 0.0;
		}
}

}}

TEST(VIENNA_CL_TEST, QR_FACTORIZATION) {
	BMatrix m(16,32); //underdetermined system
	BMatrix Q(16,16);
	BMatrix R(16,32);

	viennacl::matrix<float> vclM(16, 32);

	CS::test::testFillMatrices(m, Q, R);
	BMatrix mCopy(m);

	viennacl::copy(m, vclM);
	std::vector<float> betas = viennacl::linalg::inplace_qr(vclM);

	//copy back to CPU
	viennacl::copy(vclM, m);

	viennacl::linalg::recoverQ(m, betas, Q, R);

	BMatrix QR_product = boost::numeric::ublas::prod(Q, R);

	double maxError = CS::test::TestUtils::maxRelativeError(QR_product, mCopy);
	EXPECT_LT(maxError, 0.01);
}
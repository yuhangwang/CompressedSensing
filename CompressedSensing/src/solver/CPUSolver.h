#ifndef CPUSOLVER_H
#define CPUSOLVER_H

#include "src/solver/ISolver.h"

namespace CS {
namespace solver {
namespace cpu {

class CPUSolver : public ISolver
{
public:
    CPUSolver();
    ~CPUSolver();
    void createBinaryMeasurementMatrix(int rows, int cols, cv::Mat*output);
    cv::Mat product(const cv::Mat& A, const cv::Mat& y);
    cv::Mat transProduct(const cv::Mat& A, const cv::Mat& y);
    cv::Mat linsolve(const cv::Mat& A,const cv::Mat& y);

private:
    float setOneZero(float input);
};

}}}

#endif // CPUSOLVER_H

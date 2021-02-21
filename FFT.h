/*
 *  modified from: http://numbers.computation.free.fr/Constants/Programs/FFT.h
 */
#ifndef FFT_H
#define FFT_H
#include <vector>
using Real = double;
extern double FFTSquareWorstError;
extern long AllocatedMemory;
void InitializeFFT(long maxFFTLength);
void MulWithFFT(std::vector<Real> &aCoef, std::vector<Real> &bCoef,
                std::vector<Real> &cCoef);

#endif
#include "FFT.hpp"
#include <cassert>
#include <complex>
#include <iostream>
#include <math.h>
#include <vector>
using std::complex;
using std::vector;
double FFTSquareWorstError;
long AllocatedMemory;
const double PI = 3.1415926535897932384626;
void InitializeFFT(long Size) {}
void RecursiveFFT(vector<complex<double>> &Coef, vector<complex<double>> &FFT)
{
  assert(Coef.size() == FFT.size());
  long CoefSize = (long)Coef.size();
  if (CoefSize == 1)
  {
    FFT[0] = Coef[0];
    return;
  }
  long half = CoefSize / 2;
  vector<complex<double>> evenCoef(half), oddCoef(half), evenFFT(half),
      oddFFT(half);

  double arg = -2.0 * PI / (double)CoefSize;
  for (long i = 0; i < half; i++)
  {
    evenCoef[i] = Coef[i] + Coef[half + i];
    auto tmp = (Coef[i] - Coef[half + i]);
    double wr = cos(arg * i);
    double wi = sin(arg * i);

    oddCoef[i].real(tmp.real() * wr - tmp.imag() * wi);
    oddCoef[i].imag(tmp.real() * wi + tmp.imag() * wr);
  }
  RecursiveFFT(evenCoef, evenFFT);
  RecursiveFFT(oddCoef, oddFFT);
  for (long i = 0; i < half; i++)
  {
    FFT[2 * i] = evenFFT[i];
    FFT[2 * i + 1] = oddFFT[i];
  }
}
/* Compute the inverse Fourier Transform of FFT into Coef */
void InverseFFT(vector<complex<double>> &FFT, vector<double> &Coef)
{
  assert(FFT.size() == Coef.size());

  double N = (double)Coef.size();
  vector<complex<double>> complexCoef(N);
  for (auto &x : FFT)
  {
    x = std::conj(x);
  }
  RecursiveFFT(FFT, complexCoef);
  for (auto &x : complexCoef)
  {
    x = std::conj(x) / N;
  }
  for (long i = 0; i < N; i++)
  {
    /* Closest integer to ComplexCoef[i].R/NFFT */
    double tmp = complexCoef[i].real();
    Coef[i] = floor(0.5 + tmp);
    if ((tmp - Coef[i]) * (tmp - Coef[i]) > FFTSquareWorstError)
      FFTSquareWorstError = (tmp - Coef[i]) * (tmp - Coef[i]);
  }
}

void Convolution(vector<complex<double>> &A, vector<complex<double>> &B,
                 long NFFT, vector<complex<double>> &C)
{
  long i;
  Real tmpR, tmpI;

  for (i = 0; i < NFFT; i++)
  {
    tmpR = A[i].real() * B[i].real() - A[i].imag() * B[i].imag();
    tmpI = A[i].real() * B[i].imag() + A[i].imag() * B[i].real();
    C[i].real(tmpR);
    C[i].imag(tmpI);
  }
}

void MulWithFFT(vector<double> &ACoef, long ASize, vector<double> &BCoef,
                long BSize, vector<double> &CCoef)
{
  long NFFT = 2;
  // long ASize = ACoef.size();
  // long BSize = BCoef.size();

  while (NFFT < ASize + BSize + 1)
    NFFT *= 2;
  CCoef.resize(NFFT);
  vector<complex<double>> XCoef(NFFT, (0, 0)), YCoef(NFFT, (0, 0)), XFFT(NFFT),
      YFFT(NFFT), ZFFT(NFFT);
  for (long i = 0; i < ASize; i++)
  {
    XCoef[i].real(ACoef[i]);
  }
  for (long i = 0; i < BSize; i++)
  {
    YCoef[i].real(BCoef[i]);
  }

  RecursiveFFT(XCoef, XFFT);
  RecursiveFFT(YCoef, YFFT);
  Convolution(XFFT, YFFT, NFFT, ZFFT);
  InverseFFT(ZFFT, CCoef);
}

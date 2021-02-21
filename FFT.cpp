/*
 *  modified from: http://numbers.computation.free.fr/Constants/Programs/FFT.c
 */
#include "FFT.h"
#include <complex>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
using std::complex;
using std::vector;

const double PI = 3.1415926535897932384626;
long FFTLengthMax;
vector<complex<double>> omegaFFT;
vector<complex<double>> arrayFFT0, arrayFFT1;
vector<complex<double>> complexCoef;
double FFTSquareWorstError;
long AllocatedMemory;

void InitializeFFT(long maxLength)
{
  long i;
  Real Step;

  FFTLengthMax = maxLength;
  omegaFFT.resize(maxLength / 2);
  arrayFFT0.resize(maxLength);
  arrayFFT1.resize(maxLength);
  complexCoef.resize(maxLength);
  Step = 2. * PI / (double)maxLength;
  for (i = 0; 2 * i < maxLength; i++)
  {
    omegaFFT[i] = complex<double>(cos(Step * (double)i), sin(Step * (double)i));
  }
  FFTSquareWorstError = 0.;
  AllocatedMemory = 7 * maxLength * sizeof(complex<double>) / 2;
}

// [start,end)
void RecursiveFFT(vector<complex<double>> &Coef, long CoefOffset,
                  vector<complex<double>> &FFT, long start, long end, long Step,
                  long Sign)
{
  long i, OmegaStep;
  vector<complex<double>> Omega;
  Real tmpR, tmpI;
  long Length = end - start;
  if (Length == 2)
  {
    FFT[start].real(Coef[CoefOffset].real() + Coef[CoefOffset + Step].real());
    FFT[start].imag(Coef[CoefOffset].imag() + Coef[CoefOffset + Step].imag());

    FFT[start + 1].real(Coef[CoefOffset].real() -
                        Coef[CoefOffset + Step].real());
    FFT[start + 1].imag(Coef[CoefOffset].imag() -
                        Coef[CoefOffset + Step].imag());

    return;
  }

  long mid = (start + end) / 2;
  RecursiveFFT(Coef, CoefOffset, FFT, start, mid, Step * 2, Sign);

  RecursiveFFT(Coef, CoefOffset + Step, FFT, mid, end, Step * 2, Sign);

  Omega = omegaFFT;
  OmegaStep = FFTLengthMax / Length;
  for (i = 0; 2 * i < Length; i++)
  {
    /* Recursion formula for FFT :
       FFT[i]          <-  FFT0[i] + Omega*FFT1[i]
       FFT[i+Length/2] <-  FFT0[i] - Omega*FFT1[i],
       Omega = exp(2*I*PI*i/Length) */
    tmpR = Omega[i * OmegaStep].real() * FFT[mid + i].real() -
           Sign * Omega[i * OmegaStep].imag() * FFT[mid + i].imag();
    tmpI = Omega[i * OmegaStep].real() * FFT[mid + i].imag() +
           Sign * Omega[i * OmegaStep].imag() * FFT[mid + i].real();
    FFT[mid + i].real(FFT[start + i].real() - tmpR);
    FFT[mid + i].imag(FFT[start + i].imag() - tmpI);

    FFT[start + i].real(FFT[start + i].real() + tmpR);
    FFT[start + i].imag(FFT[start + i].imag() + tmpI);
  }
}

/* Compute the complex Fourier Transform of Coef into FFT */
void FFT(vector<double> &Coef, long Length, vector<complex<double>> &FFT,
         long NFFT)
{
  long i;
  /* Transform array of real coefficient into array of complex */
  for (i = 0; i < Length; i++)
  {
    complexCoef[i].real(Coef[i]);
    complexCoef[i].imag(0);
  }
  for (; i < NFFT; i++)
  {
    complexCoef[i].real(0);
    complexCoef[i].imag(0);
  }
  RecursiveFFT(complexCoef, 0, FFT, 0, NFFT, 1, 1);
}

/* Compute the inverse Fourier Transform of FFT into Coef */
void InverseFFT(vector<complex<double>> &FFT, long NFFT, vector<double> &Coef,
                long Length)
{
  long i;
  Real invNFFT = 1. / (Real)NFFT, tmp;

  RecursiveFFT(FFT, 0, complexCoef, 0, NFFT, 1, -1);
  for (i = 0; i < Length; i++)
  {
    /* Closest integer to ComplexCoef[i].R/NFFT */
    tmp = invNFFT * complexCoef[i].real();
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

void MulWithFFT(vector<double> &ACoef, long ASize, vector<double> &BCoef, long BSize, vector<double> &CCoef)
{
  long NFFT = 2;
  // long ASize = ACoef.size();
  // long BSize = BCoef.size();

  while (NFFT < ASize + BSize)
    NFFT *= 2;

  if (NFFT > FFTLengthMax)
  {
    printf("Error, FFT Size is too big in MulWithFFT\n");
  }
  FFT(ACoef, ASize, arrayFFT0, NFFT);
  FFT(BCoef, BSize, arrayFFT1, NFFT);
  Convolution(arrayFFT0, arrayFFT1, NFFT, arrayFFT0);
  InverseFFT(arrayFFT0, NFFT, CCoef, ASize + BSize - 1);
}

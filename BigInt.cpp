/*
 *  modified from:
 * http://numbers.computation.free.fr/Constants/Programs/BigInt.c
 */

#include "BigInt.hpp"

#include "FFT.hpp"

#include <cassert>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

using std::cerr;
using std::cout;
using std::endl;
using std::vector;

void InitializeBigInt(BigInt &A, long MaxSize)
{
  A.Coef.resize(1);
  AllocatedMemory += MaxSize * sizeof(double);
  A.Size = 0;
  A.SizeMax = MaxSize;
}
void DumpBigInt(BigInt &A)
{
  cerr << "=======================" << endl;
  cerr << "Size:" << A.Size << endl;
  cerr << "SizeMax:" << A.SizeMax << endl;
  for (int i = 0; i < A.Size; i++)
  {
    cerr << "Coef " << i << " :" << A.Coef[i] << endl;
  }
}

void FlipCoef(BigInt &A)
{
  for (int i = A.Size - 1; i >= 0; i--)
  {
    A.Coef[i] = -A.Coef[i];
  }
}
void PrintBigInt(BigInt &A, int dotPos)
{
  long i, j, Digit = 0, Dec;
  double Pow, Coef;

  printf("%.0lf", A.Coef[A.Size - 1]);

  for (i = A.Size - 2; i >= 0; i--)
  {

    if (dotPos >= 0)
    {
      dotPos--;
    }
    if (dotPos == 0)
    {
      printf(".");
    }

    Pow = BASE * 0.1;
    Coef = A.Coef[i];
    for (j = 0; j < NBDEC_BASE; j++)
    {
      Dec = (long)(Coef / Pow);
      Coef -= Dec * Pow;
      Pow *= 0.1;
      printf("%ld", Dec);
      Digit++;
    }
  }
  printf("\n");
}

/*
 * Update A to the normal form (0<= A.Coef[i] < BASE)
 */
void UpdateBigInt(BigInt &A)
{
  long i;
  Real carry = 0., x;

  for (i = 0; i < A.Size; i++)
  {
    x = A.Coef[i] + carry;
    carry = floor(x * invBASE);
    A.Coef[i] = x - carry * BASE;
  }
  if (carry != 0)
  {
    while (carry != 0.)
    {
      if (i == A.Coef.size())
      {
        A.Coef.push_back(0);
      }
      x = carry;
      carry = floor(x * invBASE);
      A.Coef[i] = x - carry * BASE;
      i++;
      A.Size = i;
    }
    if (A.Size > A.SizeMax)
    {

      printf("Error in UpdateBigInt, Size>SizeMax\n");
      printf("Size: %ld, SizeMax: %ld\n", A.Size, A.SizeMax);
    }
  }
  else
  {
    while (i > 0 && A.Coef[i - 1] == 0.)
      i--;
    A.Size = i;
  }
}

/*
 * Compute C = A + B
 */
void AddBigInt(BigInt &A, BigInt &B, BigInt &C)
{
  long i;

  if (A.Size < B.Size)
  {
    AddBigInt(B, A, C);
    assert(C.Size <= C.Coef.size());
    return;
  }
  C.Coef.resize(A.Size);
  /* We now have B.Size<A.Size */
  for (i = 0; i < B.Size; i++)
    C.Coef[i] = A.Coef[i] + B.Coef[i];
  for (; i < A.Size; i++)
    C.Coef[i] = A.Coef[i];

  C.Size = A.Size;
  assert(C.Size <= C.Coef.size());
  UpdateBigInt(C);
  assert(C.Size <= C.Coef.size());
}

/*
 * Compute C = A*B
 */
void MulBigInt(BigInt &A, BigInt &B, BigInt &C)
{
  MulWithFFT(A.Coef, A.Size, B.Coef, B.Size, C.Coef);
  C.Size = A.Size + B.Size - 1;
  C.SizeMax = C.Size + 10;
  C.Coef.resize(C.SizeMax);
  UpdateBigInt(C);
}

void TrimBigInt(BigInt &from, BigInt &to, long start, long end)
{

  InitializeBigInt(to, end - start);
  to.Size = end - start;
  for (long i = start; i < end; i++)
  {
    to.Coef.at(i - start) = from.Coef.at(i);
  }
}

/*
 * Compute the inverse of A in B in given precision (-1:same precision as
 * SizeMax of A)
 */
void Inverse(BigInt &A, BigInt &B, BigInt &tmpBigInt, int precision)
{
  long double x;
  long i, N, NN, Delta;
  int Twice = 1, Sign;
  BigInt AA;

  if (precision == -1)
  {
    precision = A.SizeMax - 10;
  }
  int oldASize = A.Size;
  int shiftWidth = precision - oldASize;
  assert(oldASize <= precision);
  A.Size = precision;
  for (int i = 0; i < A.Size; i++)
  {
    if (i < oldASize)
    {
      A.Coef[A.Size - 1 - i] = A.Coef[A.Size - 1 - i - shiftWidth];
    }
    else
    {
      A.Coef[A.Size - 1 - i] = 0;
    }
  }

  /* Initialization */
  x = A.Coef[A.Size - 1] +
      invBASE * (A.Coef[A.Size - 2] + invBASE * A.Coef[A.Size - 3]);
  x = BASE / x;
  B.Coef[1] = floor(x);
  B.Coef[0] = floor((x - B.Coef[1]) * BASE);
  B.Size = 2;

  /* Iteration used : B <- B+(1-AB)*B */
  N = 2;
  while (N < A.Size)
  {
    /* Use only the first 2*N limbs of A */
    NN = 2 * N;
    if (NN > A.Size)
      NN = A.Size;
    TrimBigInt(A, AA, A.Size - NN, A.Size);
    AA.Size = NN;
    MulBigInt(AA, B, tmpBigInt);

    // printf("%ld %ld %ld\n", AA.Size, AA.Coef.size(), tmpBigInt.Size);
    Delta = NN + B.Size - 1;
    /* Compute BASE^Delta-tmpBigInt in tmpBigInt */
    if (tmpBigInt.Size == Delta)
    {
      Sign = 1;
      for (i = 0; i < Delta; i++)
        tmpBigInt.Coef[i] = BASE - 1 - tmpBigInt.Coef[i];
      UpdateBigInt(tmpBigInt);
    }
    else
    {
      Sign = -1;
      tmpBigInt.Coef[Delta] = 0.;
    }

    MulBigInt(tmpBigInt, B, tmpBigInt);
    for (i = 0; i < tmpBigInt.Size - 2 * N + 1; i++)
      tmpBigInt.Coef[i] = tmpBigInt.Coef[i + 2 * N - 1];
    tmpBigInt.Size -= 2 * N - 1;
    for (i = B.Size - 1; i >= 0; i--)
      B.Coef[i + NN - N] = B.Coef[i];
    for (i = NN - N - 1; i >= 0; i--)
      B.Coef[i] = 0.;
    B.Size += NN - N;
    if (Sign == -1)
    {
      for (i = 0; i < tmpBigInt.Size; i++)
        tmpBigInt.Coef[i] = -tmpBigInt.Coef[i];
    }
    AddBigInt(B, tmpBigInt, B);
    /* Once during the process, redo one iteration with same precision */
    if (8 * N > A.Size && Twice)
    {
      Twice = 0;
      B.Size = N;
      for (i = 0; i < N; i++)
        B.Coef[i] = B.Coef[i + N];
    }
    else
      N *= 2;
  }
}

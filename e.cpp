/*
 * Xavier Gourdon : Sept. 99 (xavier.gourdon.free.fr)
 *
 * e.c : Basic file for computation of e=2.7181... with the binary
 *       splitting method.
 *       It relies on the files FFT.c and BigInt.c to handle
 *       Large Integers.
 *
 * Several optimizations can be made :
 *  - Use a specialized routine to handle binary splitting when the
 *    indexes are close.
 *  ...
 *
 *  Informations can be found on
 *    http://xavier.gourdon.free.fr/Constants/constants.html
 */

#include "BigInt.h"
#include "FFT.h"

#include <math.h>
#include <stdio.h>
#include <time.h>

BigInt tmpBigInt; /* Temporary BigInt used in the binary splitting */

/*
 * Compute the numerator P and denominator Q of
 * P/Q = 1/(N0+1) + 1/(N0+1)/(N0+2) + ... + 1/(N0+1)/.../N1
 */
void BinarySplittingE(long N0, long N1, BigInt *P, BigInt *Q) {
  BigInt PP, QQ;
  long NMid;

  if (N1 - N0 == 1) {
    P->Size = Q->Size = 1;
    P->Coef[0] = 1.;
    Q->Coef[0] = (double)N1;
    UpdateBigInt(P);
    UpdateBigInt(Q);
    return;
  }
  NMid = (N0 + N1) / 2;
  BinarySplittingE(N0, NMid, P, Q);
  /* To save memory, take the non used coefficients of P and Q
     for coefficient of the BigInt used in the second splitting part */
  TrimBigInt(P, &PP, P->Size, P->SizeMax);
  TrimBigInt(Q, &QQ, Q->Size, Q->SizeMax);

  BinarySplittingE(NMid, N1, &PP, &QQ);
  MulBigInt(P, &QQ, &tmpBigInt);
  AddBigInt(&tmpBigInt, &PP, P);

  MulBigInt(Q, &QQ, Q);
}

/*
 * Returns as a BigInt the constant E with NbDec decimal digits
 */
BigInt ECompute(long NbDec) {
  BigInt P, Q, tmp;
  long i, MaxSize, MaxFFTSize, SeriesSize;
  double logFactorial, logMax;

  /* MaxSize should be more than NbDec/NBDEC_BASE (see BinarySplitting) */
  MaxSize = NbDec / NBDEC_BASE + 10 + (long)(2. * log((double)NbDec) / log(2.));
  InitializeBigInt(&P, MaxSize);
  InitializeBigInt(&Q, MaxSize);

  MaxFFTSize = 2;
  while (MaxFFTSize < MaxSize)
    MaxFFTSize *= 2;
  MaxFFTSize *= 2;
  InitializeFFT(MaxFFTSize);
  /* Temporary BigInts are needed */
  InitializeBigInt(&tmpBigInt, MaxFFTSize);
  InitializeBigInt(&tmp, MaxFFTSize);

  printf("Total Allocated memory = %ld K\n", AllocatedMemory / 1024);

  /* Compute the number of term SeriesSize of the series required.
     SeriesSize is such that log(SeriesSize !) > NbDec*log(10.) */
  SeriesSize = 1;
  logFactorial = 0.;
  logMax = (double)NbDec * log(10.);
  while (logFactorial < logMax) {
    logFactorial += log((double)SeriesSize);
    SeriesSize++;
  }

  printf("Starting series computation\n");
  /* Compute the numerator P and the denominator Q of
     sum_{i=0}^{SeriesSize} 1/i! */
  BinarySplittingE(0, SeriesSize, &P, &Q);
  AddBigInt(&P, &Q, &P);
  printf("Starting final division\n");
  /* Compute the inverse of Q in tmpBigInt */
  Inverse(&Q, &tmpBigInt, &tmp);
  /* Comute P/Q in tmpBigInt */
  MulBigInt(&P, &tmpBigInt, &tmpBigInt);

  /* Put the number of required digits in P */
  P.Size = 1 + NbDec / NBDEC_BASE;
  for (i = 1; i <= P.Size; i++)
    P.Coef[P.Size - i] = tmpBigInt.Coef[tmpBigInt.Size - i];

  return P;
}

int main() {
  double StartTime;
  BigInt E;
  long NbDec;

  printf("*** E computation *** \n\n");
  printf("Enter the number of decimal digits : ");
  scanf("%ld", &NbDec);
  printf("\n");
  StartTime = (double)clock();
  E = ECompute(NbDec);
  printf("\n");
  printf("Total time : %.2lf seconds\n",
         ((double)clock() - StartTime) / CLOCKS_PER_SEC);
  printf("Worst error in FFT (should be less than 0.25): %.10lf\n",
         sqrt(FFTSquareWorstError));
  printf("\n");
  printf("E = ");
  PrintBigInt(&E);
  printf("\n");
}

/*
 *  modified from: http://numbers.computation.free.fr/Constants/Programs/BigInt.h
 */

#ifndef BigInt_h
#define BigInt_h
#include <vector>
struct BigInt
{
  std::vector<double> Coef;
  long Size, SizeMax;
};
constexpr long BASE = 10000;
constexpr double invBASE = 0.0001;
constexpr long NBDEC_BASE = 4;
void InitializeBigInt(BigInt *A, long MaxSize);
void TrimBigInt(BigInt *from, BigInt *to, long start, long end);

void PrintBigInt(BigInt *A);

void UpdateBigInt(BigInt *A);
void AddBigInt(BigInt *A, BigInt *B, BigInt *C);
void MulBigInt(BigInt *A, BigInt *B, BigInt *C);
void Inverse(BigInt *A, BigInt *B, BigInt *tmpBigInt);

#endif
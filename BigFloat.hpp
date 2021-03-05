#ifndef BIGFLOAT_HPP
#define BIGFLOAT_HPP
#include "BigInt.hpp"
struct BigFloat {
  bool sign;
  int exp;
  BigInt mantissa;
};

void AddBigFloat(BigFloat &A, BigFloat &B, BigFloat &C);
void MulBigInt(BigFloat &A, BigFloat &B, BigFloat &C);
void SubBigInt(BigFloat &A, BigFloat &B, BigFloat &C);
void DivideBigInt(BigFloat &A, BigFloat &B, BigFloat &C);
void Inverse(BigFloat &A, BigFloat &B, BigFloat &tmp, int = -1);
void DumpBigInt(BigFloat &A);
void PrintBigInt(BigFloat &A);

#endif
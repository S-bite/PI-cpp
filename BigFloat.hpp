#ifndef BIGFLOAT_HPP
#define BIGFLOAT_HPP
#include "BigInt.hpp"
constexpr bool POSI = true;
constexpr bool NEGA = false;
struct BigFloat
{
  bool sign;
  int exponent;
  BigInt fraction;
};

int CompareAbs(BigFloat &A, BigFloat &B);
void InitializeBigFloat(BigFloat &A, bool sign = POSI, int exponent = 0,
                        int fraction = 0);
void changeExp(BigFloat &A, int exp);
void AddBigFloat(BigFloat &A, BigFloat &B, BigFloat &C);
void MulBigFloat(BigFloat &A, BigFloat &B, BigFloat &C);
void SubBigFloat(BigFloat &A, BigFloat &B, BigFloat &C);
void DivideBigFloat(BigFloat &A, BigFloat &B, BigFloat &C);
void Inverse(BigFloat &A, BigFloat &B);
void DumpBigFloat(BigFloat &A);
void PrintBigFloat(BigFloat &A);

#endif
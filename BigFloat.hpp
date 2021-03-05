#ifndef BIGFLOAT_HPP
#define BIGFLOAT_HPP
#include "BigInt.hpp"
constexpr bool POSI = true;
constexpr bool NEGA = true;

// BigFloat AはA.sign*(BASE^A.exponent)*A.fractionを表す
struct BigFloat {
  bool sign;
  int exponent;
  BigInt fraction;
};

void InitializeBigFloat(BigFloat &A, bool sign = POSI, int exponent = 0,
                        int fraction = 0);
void AddBigFloat(BigFloat &A, BigFloat &B, BigFloat &C);
void MulBigFloat(BigFloat &A, BigFloat &B, BigFloat &C);
void SubBigFloat(BigFloat &A, BigFloat &B, BigFloat &C);
void DivideBigFloat(BigFloat &A, BigFloat &B, BigFloat &C);
void Inverse(BigFloat &A, BigFloat &B, BigFloat &tmp, int = -1);
void DumpBigFloat(BigFloat &A);
void PrintBigFloat(BigFloat &A);

#endif
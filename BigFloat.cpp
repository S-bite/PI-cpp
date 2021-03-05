#include "BigFloat.hpp"
#include "BigInt.hpp"
#include <cassert>
#include <iostream>
#include <vector>

void InitializeBigFloat(BigFloat &A, bool sign, int exponent, int fraction) {
  InitializeBigInt(A.fraction, 100);
  A.exponent = exponent;
  A.sign = sign;
  A.fraction.Size = 1;
  A.fraction.Coef[0] = fraction;
  UpdateBigInt(A.fraction);
}
void changeExp(BigFloat &A, int exp) {
  int diff = exp - A.exponent;
  A.exponent = exp;
  if (diff > 0) {
    // expが増える→A.fractionを小さくする
    A.fraction.Coef = std::vector<double>(A.fraction.Coef.begin() + diff,
                                          A.fraction.Coef.end());
    A.fraction.Size -= diff;
  } else {
    // expが減る→A.fractionを大きくする
    diff = -diff;
    A.fraction.Coef.insert(A.fraction.Coef.begin(), diff, 0);
    A.fraction.Size += diff;
    assert(A.fraction.Size <= A.fraction.SizeMax);
  }
}

void AddBigFloat(BigFloat &A, BigFloat &B, BigFloat &C) {
  if (A.sign == B.sign) {
    C.sign = A.sign;
    if (A.exponent > B.exponent) {
      int oldExponent = A.exponent;
      changeExp(A, B.exponent);
      AddBigInt(A.fraction, B.fraction, C.fraction);
      C.exponent = B.exponent;
      A.exponent = oldExponent;
    } else {
      int oldExponent = B.exponent;
      changeExp(B, A.exponent);
      AddBigInt(A.fraction, B.fraction, C.fraction);
      C.exponent = A.exponent;
      B.exponent = oldExponent;
    }
  } else {
    std::cerr << "Not Implemented" << std::endl;
    assert(false);
  }
}
void MulBigFloat(BigFloat &A, BigFloat &B, BigFloat &C) {}
void SubBigFloat(BigFloat &A, BigFloat &B, BigFloat &C) {}
void DivideBigFloat(BigFloat &A, BigFloat &B, BigFloat &C) {}
void Inverse(BigFloat &A, BigFloat &B, BigFloat &tmp) {}
void DumpBigFloat(BigFloat &A) {}
void PrintBigFloat(BigFloat &A) {
  // TODO 表示がバグるのを直す
  std::cout << (A.sign == POSI ? "" : "-");
  PrintBigInt(A.fraction, -A.exponent);
}

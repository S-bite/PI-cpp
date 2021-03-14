#include "BigFloat.hpp"
#include "BigInt.hpp"
#include <cassert>
#include <iostream>
#include <vector>
#include <cmath>
using std::cerr;
using std::cout;
using std::endl;

void InitializeBigFloat(BigFloat &A, bool sign, int exponent, long long fraction)
{
  assert(fraction >= 0);
  InitializeBigInt(A.fraction, 10000);
  A.exponent = exponent;
  A.sign = sign;
  A.fraction.Size = 1;
  A.fraction.Coef[0] = fraction;
  UpdateBigInt(A.fraction);
}

/*
  compare absolute value of given BigFloat A,B
  return value > 0 if abs(A) > abs(B)
  return value < 0 if abs(A) < abs(B)
  return value = 0 if abs(A) = abs(B)
*/
int CompareAbs(BigFloat &A, BigFloat &B)
{
  if (A.exponent + A.fraction.Size > B.exponent + B.fraction.Size)
  {
    return 1;
  }
  else if (A.exponent + A.fraction.Size < B.exponent + B.fraction.Size)
  {
    return -1;
  }
  else
  {
    int i = A.fraction.Size - 1;
    int j = B.fraction.Size - 1;

    for (; i >= 0 || j >= 0; i--, j--)
    {
      int a = (i < 0 ? 0 : A.fraction.Coef[i]);
      int b = (j < 0 ? 0 : B.fraction.Coef[j]);
      if (a > b)
      {
        return 1;
      }
      else if (a < b)
      {
        return -1;
      }
    }
    return 0;
  }
}

void changeExp(BigFloat &A, int exp)
{
  // cerr << "1=============================" << endl;
  // cerr << exp << " " << A.fraction.Coef.size() << endl;
  int diff = exp - A.exponent;
  A.exponent = exp;
  if (diff > 0)
  {
    //assert(false);
    // expが増える→A.fractionを小さくする
    A.fraction.Coef = std::vector<double>(A.fraction.Coef.begin() + diff,
                                          A.fraction.Coef.end());
    A.fraction.Size -= diff;
    //PrintBigFloat(A);
    // cerr << "2=============================" << endl;
  }
  else
  {
    // cerr << exp << " " << A.fraction.Coef.size() << endl;

    // expが減る→A.fractionを大きくする
    diff = -diff;
    A.fraction.Coef.insert(A.fraction.Coef.begin(), diff, 0);
    A.fraction.Size += diff;
    assert(A.fraction.Size <= A.fraction.SizeMax);
  }
}
void changePrecision(BigFloat &A, int prec)
{
  if (prec > A.fraction.Size)
  {
    return;
  }
  int diff = A.fraction.Size - prec;
  A.exponent += diff;
  for (int i = 0; i < prec; i++)
  {
    A.fraction.Coef[i] = A.fraction.Coef[i + diff];
  }
  A.fraction.Size = prec;
  A.fraction.Coef.resize(prec);
  assert(A.fraction.Size <= A.fraction.Coef.size());
  assert(A.fraction.Coef.size() <= A.fraction.SizeMax);
}
void shrink(BigFloat &A)
{
  int diff = 0;
  while (A.fraction.Coef[diff] == 0)
    diff++;
  changeExp(A, A.exponent + diff);
}
void AddBigFloat(BigFloat A, BigFloat B, BigFloat &C)
{
  if (A.sign == B.sign)
  {
    C.sign = A.sign;
    if (A.exponent > B.exponent)
    {
      int oldExponent = B.exponent;
      changeExp(A, B.exponent);
      AddBigInt(A.fraction, B.fraction, C.fraction);
      C.exponent = B.exponent;
      changeExp(A, oldExponent);
    }
    else
    {
      int oldExponent = B.exponent;
      changeExp(B, A.exponent);
      AddBigInt(A.fraction, B.fraction, C.fraction);
      // cerr << "--------" << endl;
      // PrintBigFloat(A);
      // PrintBigFloat(B);

      // cerr << "--------" << endl;

      C.exponent = A.exponent;
      changeExp(B, oldExponent);
    }
  }
  else
  {
    if (CompareAbs(A, B) > 0)
    {
      B.sign = !B.sign;
      FlipCoef(B.fraction);
      AddBigFloat(A, B, C);
      FlipCoef(B.fraction);
      B.sign = !B.sign;
    }
    else
    {
      A.sign = !A.sign;
      FlipCoef(A.fraction);
      AddBigFloat(A, B, C);
      FlipCoef(A.fraction);
      A.sign = !A.sign;
    }
  }
  UpdateBigInt(C.fraction);
}

double toDouble(BigFloat &A)
{
  double ret = 0;
  for (int i = A.fraction.Size - 1; i >= 0; i--)
  {
    ret += std::pow(BASE, A.exponent + i) * A.fraction.Coef[i];
  }
  if (A.sign == NEGA)
  {
    ret = -ret;
  }
  return ret;
}
void MulBigFloat(BigFloat A, BigFloat B, BigFloat &C)
{
  C.sign = !(A.sign ^ B.sign);
  if (A.exponent > B.exponent)
  {
    int oldExponent = B.exponent;
    changeExp(A, B.exponent);
    MulBigInt(A.fraction, B.fraction, C.fraction);
    C.exponent = A.exponent + B.exponent;
    changeExp(A, oldExponent);
  }
  else
  {
    int oldExponent = B.exponent;
    changeExp(B, A.exponent);
    MulBigInt(A.fraction, B.fraction, C.fraction);
    C.exponent = A.exponent + B.exponent;
    changeExp(B, oldExponent);
  }
  shrink(C);
}
void SubBigFloat(BigFloat A, BigFloat B, BigFloat &C)
{
  B.sign = !B.sign;
  AddBigFloat(A, B, C);
  B.sign = !B.sign;
  //UpdateBigInt(C.fraction);
}
void DivideBigFloat(BigFloat A, BigFloat B, BigFloat &C)
{
  BigFloat tmp;
  while (A.exponent > 0 && B.exponent > 0)
  {
    A.exponent--;
    B.exponent--;
  }
  InitializeBigFloat(tmp, POSI, 0, 0);
  cerr << "Inverse" << endl;
  Inverse(B, tmp);
  cerr << "Mul" << endl;
  changePrecision(tmp, tmp.fraction.SizeMax / 2 - 1);
  changePrecision(A, A.fraction.SizeMax / 2 - 1);

  MulBigFloat(A, tmp, C);
  PrintBigFloat(C);
}
void Inverse(BigFloat A, BigFloat &B)
{
  cerr << "Invese()" << endl;
  PrintBigFloat(A);
  BigFloat one, tmp, tmp2;
  InitializeBigFloat(one, POSI, 0, 1);
  InitializeBigFloat(tmp, POSI, 0, 1);
  InitializeBigFloat(tmp2, POSI, 0, 1);
  changePrecision(A, 100);
  PrintBigFloat(A);
  long long init = (long long)((1.0 / toDouble(A)) * (double)BASE * (double)BASE);
  cerr << "init " << toDouble(A) << endl;
  //  cerr << (int)((1.0 / toDouble(A)) * (double)BASE) << endl;
  //InitializeBigFloat(B, POSI, -20, 6000);
  InitializeBigFloat(B, POSI, -12, 6000);
  PrintBigFloat(B);
  auto back = B.fraction;
  for (int i = 0;; i++)
  {
    PrintBigFloat(B);
    //B = B + B * (1 - A * B);
    MulBigFloat(A, B, tmp);
    SubBigFloat(one, tmp, tmp);
    MulBigFloat(B, tmp, tmp);
    AddBigFloat(B, tmp, B);
    UpdateBigInt(B.fraction);
    if (B.fraction == back)
    {
      break;
    }
    //  PrintBigFloat(B);
    back = B.fraction;
    changePrecision(B, 200);
  }
  // cerr << "done" << endl;
}

void InverseSqrt(BigFloat A, BigFloat &B)
{
  BigFloat one, half, tmp, tmp2;
  InitializeBigFloat(one, POSI, 0, 1);
  InitializeBigFloat(half, POSI, -1, 5000);
  InitializeBigFloat(tmp, POSI, 0, 1);
  InitializeBigFloat(tmp2, POSI, 0, 1);
  int init = (int)((1.0 / sqrt(toDouble(A))) * (double)BASE * (double)BASE);
  cerr << init << endl;
  //  cerr << (int)((1.0 / toDouble(A)) * (double)BASE) << endl;
  InitializeBigFloat(B, POSI, -2, init);
  PrintBigFloat(B);
  auto back = B.fraction;
  for (int i = 0; i < 4; i++)
  {
    PrintBigFloat(B);
    //B = B - 0.5 * B * (A * B * B - 1);
    //  PrintBigFloat(B);
    MulBigFloat(A, B, tmp);
    changePrecision(tmp, 100);

    MulBigFloat(B, tmp, tmp);
    changePrecision(tmp, 100);

    SubBigFloat(tmp, one, tmp);
    changePrecision(tmp, 100);

    MulBigFloat(B, tmp, tmp);
    changePrecision(tmp, 100);

    MulBigFloat(half, tmp, tmp);
    changePrecision(tmp, 100);

    SubBigFloat(B, tmp, B);
    UpdateBigInt(B.fraction);
    changePrecision(tmp, 100);

    if (B.fraction == back)
    {
      break;
    }
    //  PrintBigFloat(B);
    back = B.fraction;
    changePrecision(B, 100);
  }
  // cerr << "done" << endl;
}
void DumpBigFloat(BigFloat &A) {}

int getDigitNum(int num)
{

  if (num < 10)
  {
    return 1;
  }
  else if (num < 100)
  {
    return 2;
  }
  else if (num < 1000)
  {
    return 3;
  }
  else
  {
    assert(num < 10000);
    return 4;
  }
}
int getDigit(int num, int pos)
{

  for (int i = 0; i < pos; i++)
  {
    num /= 10;
  }
  return num % 10;
}
void PrintBigFloat(BigFloat &A)

{
  // TODO リファクタリング
  std::cout << (A.sign == POSI ? "" : "-");
  // cerr << "!" << A.exponent << endl;
  // cerr << "!" << A.fraction.Size << endl;

  int pointPos = A.exponent;
  bool didPrintNonZero = false;
  // cerr << pointPos << endl;
  if (pointPos <= 0)
  {
    pointPos = A.fraction.Size - 1 + pointPos;
    if (pointPos < 0)
    {
      //cerr << "!!!!!!!a" << endl;
      std::cout << "0.";
      for (int i = 0; i < -pointPos - 1; i++)
      {

        std::cout << "0000";
      }
      didPrintNonZero = true;
      for (int i = A.fraction.Size - 1; i >= 0; i--)
      {
        for (int j = 0; j < NBDEC_BASE; j++)
        {
          if (didPrintNonZero || getDigit(A.fraction.Coef[i], NBDEC_BASE - 1 - j) != 0)
          {
            didPrintNonZero = true;
            std::cout << getDigit(A.fraction.Coef[i], NBDEC_BASE - 1 - j);
          }
        }
      }
      std::cout << std::endl;
    }
    else
    {
      //cerr << "!!!!!!!b" << endl;

      for (int i = A.fraction.Size - 1; i >= 0; i--)
      {
        for (int j = 0; j < NBDEC_BASE; j++)
        {
          if (didPrintNonZero || getDigit(A.fraction.Coef[i], NBDEC_BASE - 1 - j) != 0)
          {
            didPrintNonZero = true;
            std::cout << getDigit(A.fraction.Coef[i], NBDEC_BASE - 1 - j);
          }
        }

        if (pointPos == 0)
        {
          cout << ".";
        }
        pointPos--;
      }
      std::cout << std::endl;
    }
  }
  else
  {
    //cerr << "!!!!!!!c" << endl;

    long long tmp = A.exponent;

    for (int i = A.fraction.Size - 1; i >= 0; i--)
    {
      for (int j = 0; j < NBDEC_BASE; j++)
      {
        if (didPrintNonZero || getDigit(A.fraction.Coef[i], NBDEC_BASE - 1 - j) != 0)
        {
          didPrintNonZero = true;
          std::cout << getDigit(A.fraction.Coef[i], NBDEC_BASE - 1 - j);
        }
      }
    }
    for (int i = 0; i < pointPos; i++)
    {
      for (int j = 0; j < NBDEC_BASE; j++)
      {
        std::cout << 0;
      }
    }
    std::cout << std::endl;
  }
}

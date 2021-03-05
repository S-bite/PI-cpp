#include "BigInt.hpp"
#include "FFT.hpp"

void test() {
  BigInt a, b, c;
  InitializeFFT(1024);
  InitializeBigInt(a, 100);
  InitializeBigInt(b, 100);
  InitializeBigInt(c, 100);
  a.Size = 1;
  a.Coef[0] = 96236657;
  UpdateBigInt(a);
  DumpBigInt(a);
  Inverse(a, b, c);
  PrintBigInt(a);
  PrintBigInt(b);
  PrintBigInt(c);
  DumpBigInt(b);
}

int main() { test(); }

#include "BigFloat.hpp"
#include "FFT.hpp"

void test()
{
  BigFloat a, b, c;
  InitializeFFT(1024);
  InitializeBigFloat(a, POSI, 2, 314151592);
  InitializeBigFloat(b, POSI, 3, 14145);
  InitializeBigFloat(c);
  PrintBigFloat(a);
  PrintBigFloat(b);

  AddBigFloat(a, b, c);
  PrintBigFloat(a);
  PrintBigFloat(b);
  PrintBigFloat(c);
}

int main() { test(); }

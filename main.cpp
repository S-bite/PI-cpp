#include "BigFloat.hpp"
#include "FFT.hpp"

void test() {
  BigFloat a, b, c;
  InitializeFFT(1024);
  InitializeBigFloat(a, POSI, -1, 31415);
  InitializeBigFloat(b, POSI, 0, 14145);
  InitializeBigFloat(c);
  PrintBigFloat(a);
  PrintBigFloat(b);
  AddBigFloat(a, b, c);
  PrintBigFloat(c);
}

int main() { test(); }

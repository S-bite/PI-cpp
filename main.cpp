#include <iostream>
#include "BigFloat.hpp"
#include "FFT.hpp"
using std::cerr;
using std::cout;
using std::endl;

void test()
{
  BigFloat a, b, c;
  InitializeFFT(1024);
  InitializeBigFloat(a, POSI, 0, 2);
  InitializeBigFloat(b, POSI, 10, 2);

  InitializeBigFloat(c);
  PrintBigFloat(a);
  // PrintBigFloat(b);
  InverseSqrt(a, c);
  cerr << "--------------------------" << endl;
  PrintBigFloat(c);
  Inverse(c, a);
  //DivideBigFloat(a, b, c);
  PrintBigFloat(a);
  InitializeBigFloat(a, POSI, 0, 2);
  MulBigFloat(c, b, a);
  PrintBigFloat(a);
  cerr << toDouble(c) << endl;
}

int main() { test(); }

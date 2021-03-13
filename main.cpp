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
  InitializeBigFloat(a, POSI, 0, 7);
  InitializeBigFloat(b, POSI, -1, 14142);
  cerr << toDouble(a) << endl;
  cerr << toDouble(b) << endl;

  InitializeBigFloat(c);
  PrintBigFloat(a);
  PrintBigFloat(b);
  Inverse(a, c);
  PrintBigFloat(c);
  cerr << toDouble(c) << endl;
}

int main() { test(); }

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
  InitializeBigFloat(a, POSI, 10, 22);
  InitializeBigFloat(b, POSI, 10, 7);

  InitializeBigFloat(c);
  PrintBigFloat(a);
  // PrintBigFloat(b);
  //Inverse(a, c);
  DivideBigFloat(a, b, c);
  //PrintBigFloat(c);
  cerr << toDouble(c) << endl;
}

int main() { test(); }

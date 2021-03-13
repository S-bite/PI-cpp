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
  InitializeBigFloat(a, POSI, -1, 31415);
  InitializeBigFloat(b, NEGA, -2, 1);
  InitializeBigFloat(c);
  PrintBigFloat(a);
  PrintBigFloat(b);
  //AddBigFloat(a, b, b);
  Inverse(a, c);
  PrintBigFloat(c);
}

int main() { test(); }

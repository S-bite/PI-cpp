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
  InitializeBigFloat(a, NEGA, -10, 1234);
  InitializeBigFloat(b, NEGA, -2, 314159265);
  InitializeBigFloat(c);
  PrintBigFloat(a);
  PrintBigFloat(b);
  cerr << "Bigger is ";
  if (CompareAbs(a, b) > 0)
  {
    PrintBigFloat(a);
  }
  if (CompareAbs(a, b) < 0)
  {
    PrintBigFloat(b);
  }
  if (CompareAbs(a, b) == 0)
  {
    cerr << "Both" << endl;
  }
  SubBigFloat(a, b, c);
  PrintBigFloat(a);
  PrintBigFloat(b);
  PrintBigFloat(c);
}

int main() { test(); }

#include <iostream>
#include <vector>
#include "testUtil.hpp"
#include "BigFloat.hpp"
#include "FFT.hpp"
using std::cerr;
using std::cout;
using std::endl;
using std::vector;

long long C3over24 = 10939058860032000;
long long A = 13591409;
long long B = 545140134;
long long C = 640320;

void PQT_merge(BigFloat &P, BigFloat &Q, BigFloat &T, BigFloat P1, BigFloat P2, BigFloat Q1, BigFloat Q2, BigFloat T1, BigFloat T2)
{
  cerr << "merge" << endl;
  InitializeBigFloat(P);
  InitializeBigFloat(Q);
  InitializeBigFloat(T);

  PrintBigFloat(P1);
  PrintBigFloat(Q1);
  PrintBigFloat(T1);

  PrintBigFloat(P2);
  PrintBigFloat(Q2);
  PrintBigFloat(T2);
  MulBigFloat(P1, P2, P);
  MulBigFloat(Q1, Q2, Q);
  MulBigFloat(T1, Q2, T);
  MulBigFloat(P1, T2, P1);

  AddBigFloat(T, P1, T);
}
void binary_splitting(BigFloat &P, BigFloat &Q, BigFloat &T, long long l, long long r)
{
  // mtx.lock();
  // cerr << this_thread::get_id() << " "
  //      << "enter binary splitting ";
  // dump(l);
  // dump(r);
  // el;
  // mtx.unlock();
  if (l > r)
  {
    InitializeBigFloat(P, POSI, 0, 1);
    InitializeBigFloat(Q, POSI, 0, 1);
    InitializeBigFloat(T, POSI, 0, 1);
  }
  else if (l + 1 == r)
  {
    BigFloat tmp, tmp2;

    InitializeBigFloat(P, POSI, 0, 2ll * r - 1ll);

    InitializeBigFloat(tmp, POSI, 0, 6 * r - 1);
    MulBigFloat(tmp, P, P);
    InitializeBigFloat(tmp, POSI, 0, 6 * r - 5);
    MulBigFloat(tmp, P, P);

    // calc Q
    InitializeBigFloat(Q, POSI, 0, C3over24);
    InitializeBigFloat(tmp, POSI, 0, r);
    MulBigFloat(tmp, Q, Q);
    MulBigFloat(tmp, Q, Q);
    MulBigFloat(tmp, Q, Q);

    // calc T

    InitializeBigFloat(tmp2, POSI, 0, B);
    MulBigFloat(tmp2, tmp, tmp2);
    InitializeBigFloat(tmp, POSI, 0, A);

    AddBigFloat(tmp2, tmp, tmp2);
    if (r % 2 == 1)
    {
      tmp2.sign = !tmp2.sign;
    }
    T = P;
    MulBigFloat(tmp2, T, T);
  }
  else
  {
    long long mid = (l + r) / 2;
    BigFloat P1, P2, Q1, Q2, T1, T2;
    //  mtx.unlock();
    binary_splitting(P1, Q1, T1, l, mid);
    binary_splitting(P2, Q2, T2, mid, r);
    PQT_merge(P, Q, T, P1, P2, Q1, Q2, T1, T2);
  }
}

/*
  a=  1234    5678 * (10000)^-2=0.3333 3333
     Coef[1] Coef[0] 
  a=3333 3333 * (10000)^-2=0.3333 3333

  a=3333 * (10000)^-1=0.3333
  
*/

void test()
{
  BigFloat a, b, c;
  InitializeBigFloat(a, POSI, 0, 3);
  Inverse(a, a);
  PrintBigFloat(a);
  InitializeBigFloat(b, POSI, 0, 231);
  InitializeBigFloat(c, POSI, 0, 0);
  MulBigFloat(a, b, c);
  PrintBigFloat(c);

  // changePrecision(a, 220);
  // PrintBigFloat(a);

  // // a =  (BASE)^0*5
  // InitializeBigFloat(b, POSI, 0, 231);
  // InitializeBigFloat(c);
}

void testIO()
{
  BigFloat A;
  vector<string> strs = {"12345.1", "123456.12", "12345678.123", "12345678.1234", "-1"};
  for (auto str : strs)
  {
    InitializeBigFloatFromString(A, str);
    PrintBigFloat(A);
    assertEqual(toString(A), str);
  }
}

int main()
{
  testIO();
  return 0;
  BigFloat P, Q, T, pi, tmp, _12, invSqrtC;
  binary_splitting(P, Q, T, 0, 5);
  cout << "P = ";
  PrintBigFloat(P);
  cout << "Q = ";
  PrintBigFloat(Q);
  cout << "T = ";
  PrintBigFloat(T);
  pi = Q;
  InitializeBigFloat(tmp, POSI, 0, C);
  MulBigFloat(tmp, pi, pi);
  MulBigFloat(tmp, pi, pi);
  InitializeBigFloat(_12, POSI, 0, 12);
  InitializeBigFloat(invSqrtC, POSI, 0, C);
  InverseSqrt(invSqrtC, invSqrtC);
  PrintBigFloat(invSqrtC);
  InitializeBigFloat(tmp, POSI, 0, A);
  MulBigFloat(tmp, Q, tmp);
  AddBigFloat(tmp, T, tmp);
  MulBigFloat(tmp, _12, tmp);
  PrintBigFloat(tmp);
  Inverse(tmp, tmp);

  PrintBigFloat(pi);
  PrintBigFloat(tmp);
  PrintBigFloat(invSqrtC);

  changePrecision(tmp, 100);
  changePrecision(invSqrtC, 100);
  MulBigFloat(tmp, invSqrtC, tmp);
  PrintBigFloat(tmp);
  MulBigFloat(tmp, pi, pi);
  cout << "PI=";
  PrintBigFloat(pi);
}

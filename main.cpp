#include "BigInt.h"
#include "FFT.h"

int main()
{
    // calculate 1234^10=8187505353567209228244052427776
    BigInt a, b;
    InitializeFFT(1024);
    InitializeBigInt(&a, 100);
    InitializeBigInt(&b, 100);

    a.Size = 1;
    a.Coef[0] = 1234;
    b.Size = 1;
    b.Coef[0] = 1;
    UpdateBigInt(&a);
    UpdateBigInt(&b);

    for (int i = 0; i < 10; i++)
    {
        MulBigInt(&a, &b, &b);
        UpdateBigInt(&b);
    }
    PrintBigInt(&b);
}

#include <stdio.h>
const int MOD = 10007;
int Power(int N, int k)
{
    if (k == 0)
        return 1;
    if (k % 2 == 0) // 如果指数k为偶数
    {
        int tmp = Power(N, k / 2);
        return (tmp * tmp) % MOD;
    }
    else // 如果为奇数
    {
        return (N * Power(N, k - 1)) % MOD;
    }
}

int main()
{
    int N, k;
    scanf("%d%d", &N, &k);
    printf("%d\n", Power(N, k));
    return 0;
}

/* Your function will be put here */

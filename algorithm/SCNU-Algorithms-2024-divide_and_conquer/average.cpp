#include <iostream>
using namespace std;
// 下面递归程序返回实数数组A[1..n]的平均值，请选择合适的表达式填空（假定参数n>0）：
double Average(double A[], int n)
{
    if (n == 1)
        return A[1];

    return Average(A, n - 1) / n + A[n] / n;
}
int main()
{
    double A[] = {1, 2, 3, 4};
    cout << Average(A, 3);
    return 0;
}
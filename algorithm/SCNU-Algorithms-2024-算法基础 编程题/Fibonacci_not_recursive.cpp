#include <iostream>
#include <vector>
using namespace std;
// 一个特定的质数，常用来避免int类型溢出
const int P = 998244353;
int Fibonicci_not_recursive(int n)
{
    vector<int> f(n + 1);
    f[1] = 1;
    f[2] = 1;
    for (int i = 3; i <= n; i++)
    {
        // f[i] = f[i - 1] + f[i - 2];
        //  该数组可能出现int类型溢出，处理如下
        f[i] = ((f[i - 1] + f[i - 2]) % P + P) % P;
    }
    return f[n];
}
int main()
{
    int n = 0;
    cin >> n;
    cout << Fibonicci_not_recursive(n) << endl;
    return 0;
}
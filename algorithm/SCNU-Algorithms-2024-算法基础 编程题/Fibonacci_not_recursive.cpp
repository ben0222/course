#include <iostream>
#include <vector>
using namespace std;
int Fibonicci_not_recursive(int n)
{
    vector<int> f(n + 1);
    f[0] = 1;
    f[1] = 1;
    for (int i = 2; i <= n; i++)
    {
        f[i] = f[i - 1] + f[i - 2];
    }
    return f[n];
}
int main()
{
    int n = 0;
    cin >> n;
    cout << Fibonicci_not_recursive(n) % 998244353 << endl;
    return 0;
}
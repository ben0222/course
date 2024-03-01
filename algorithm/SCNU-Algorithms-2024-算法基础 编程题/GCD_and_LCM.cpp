#include <iostream>
using namespace std;
int gcd(int m, int n)
{
    return n == 0 ? m : gcd(m, m % n);
}
int lcm(int m, int n)
{
    return (m * n) / gcd(m, n);
}
int main()
{
    int a, b;
    cin >> a >> b;
    cout << gcd(a, b) << " " << lcm(a, b) << endl;
    return 0;
}
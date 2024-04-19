#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const int mod = 10007;
ll quick_power(ll base, ll power)
{
    ll result = 1;
    while (power)
    {
        if (power & 1)
        {
            result = result * base % mod;
        }
        power /= 2;
        base = base * base % mod;
    }
    return result;
}
void solve()
{
    ll n, k;
    cin >> n >> k;
    cout << quick_power(n, k);
}
int main()
{
    ios::sync_with_stdio(0), cin.tie(0), cout.tie(0);
    int t = 1;
    // cin >> t;
    while (t--)
        solve();
    return 0;
}
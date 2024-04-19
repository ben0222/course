#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const int N = 1e4 + 9;
int a[N], b[N];
int x, n;
void solve()
{
    cin >> n;
    for (int i = 1; i <= n; i++)
    {
        cin >> x >> a[i];
    }
    sort(a + 1, a + 1 + n);
    for (int i = n; i >= 1; i--)
        b[n - i + 1] = a[i];
    ll s = 0;
    for (int i = 1; b[i] > a[i]; i++)
    {
        s += b[i] - a[i];
    }
    cout << s;
}
int main()
{
    ios::sync_with_stdio(0), cin.tie(0), cout.tie(0);
    int t = 1;
    // cin >> t;
    while (t--)
        solve();
    return 0;
    return 0;
}
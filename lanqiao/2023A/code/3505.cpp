// https://www.lanqiao.cn/problems/3505/learning/?subject_code=1&group_code=5&match_num=14&match_flow=1&origin=cup
#include <bits/stdc++.h>
#include <iostream>
#define endl "\n"
#define INF 0x3f3f3f3f
#define deb(x) cout << #x << "=" << x << '\n';
const int N = 100;
double a[N]; // 存储每个瓜的重量
double s[N];
int n, m;
int ans = INF;
using namespace std;
struct Compare
{
    bool operator()(int x, int y)
    {
        return x > y;
    }
};

void dfs(int u, double w, int count)
{
    if (w == m)
    {
        ans = min(ans, count);
        return;
    }
    // 如果n个瓜都遍历完了，那就返回。
    if (u >= n)
        return;

    // 如果当前方案并不优于已有的合法答案，那就返回。
    if (count > ans)
        return;

    // 如果总重量已经超了，那么当前方案不合法。
    if (w > m)
        return;

    // 如果后面的瓜的所有重量加起来，都小于m，那么当前方案不合法。
    if (w + s[u] < m)
        return;

    // 选，但是不切
    dfs(u + 1, w + a[u], count);

    // 选，但是切
    dfs(u + 1, w + a[u] / 2.0, count + 1);

    // 不选
    dfs(u + 1, w, count);
}
void solve()
{
    cin >> n >> m;
    for (int i = 0; i < n; i++)
    {
        cin >> a[i];
    }
    sort(a, a + n, Compare());
    for (int i = n - 1; i >= 0; i--)
    {
        s[i] = s[i + 1] + a[i];
    }
    dfs(0, 0.0, 0);
    if (ans == INF)
        ans = -1;
    cout << ans << endl;
}

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    int t = 1;
    while (t--)
        solve();
    return 0;
}
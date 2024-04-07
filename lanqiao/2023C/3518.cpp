// https://www.lanqiao.cn/problems/3518/learning/?subject_code=1&group_code=3&match_num=14&match_flow=1&origin=cup
#include <iostream>
#include <bits/stdc++.h>
#define int long long
#define endl '\n'
#define deb(x) cout << #x << '=' << x << '\n';
#define INF 0x3f3f3f3f
int n;
using namespace std;

int cal_ans(vector<int> &a, vector<int> &b, vector<int> &c)
{
    vector<int> d(n);

    for (int i = 0; i < n; i++)
    {
        d[i] = a[i] - (b[i] + c[i]);
    }
    sort(d.begin(), d.end()); // 降序排序

    int sum = 0;
    int count = 0;
    for (int i = n - 1; i >= 0; i--) // 贪心：为了使游戏事件尽量多，从后面（比较小的）开始 累加d[i]。
    {
        if (sum + d[i] > 0) // 用sum来实现累加, 只要sum+d[i]还是大于0就还可以加，说明事件还可以多
        {
            sum += d[i];
            count++;
        }
        else
            break;
    }
    return count;
}
void solve()
{
    cin >> n;
    vector<int> a(n), b(n), c(n);
    for (int i = 0; i < n; i++)
    {
        cin >> a[i];
    }
    for (int i = 0; i < n; i++)
    {
        cin >> b[i];
    }
    for (int i = 0; i < n; i++)
    {
        cin >> c[i];
    }

    int ans = 0;

    ans = cal_ans(a, b, c); // x赢

    ans = max(ans, cal_ans(b, a, c)); // y赢

    ans = max(ans, cal_ans(c, a, b)); // z赢

    if (ans)
        cout << ans << endl;
    else
        cout << -1 << endl;
}
signed main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    solve();
}
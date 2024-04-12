// https://www.lanqiao.cn/problems/3511/learning/?subject_code=1&group_code=4&match_num=14&match_flow=1&origin=cup
#include <bits/stdc++.h>
#include <iostream>
// #define int long long
const int N = 30;
using namespace std;

struct plane
{
    int t; // 到达时间
    int d; // 可盘旋时间
    int l; // 降落所需时间
} p[N];

bool st[N];               // 判断当前飞机是否已经降落
int n;                    // 飞机数量
bool dfs(int u, int time) // 已有u驾飞机降落，前一架飞机降落完成时间为time
{
    if (u >= n) // 退出条件
        return true;

    for (int i = 0; i < n; i++)
    {
        if (!st[i])
        {
            st[i] = true;
            if (p[i].t + p[i].d < time)
            {
                st[i] = false; // 回退到dfs前的状态
                return false;
            }

            int t = max(time, p[i].t) + p[i].l; // 更新降落后的时间进行dfs

            if (dfs(u + 1, t))
                return true;

            st[i] = false; // 回退到dfs前的状态
        }
    }
    return false;
}
void solve()
{
    cin >> n;
    for (int i = 0; i < n; i++)
    {
        cin >> p[i].t >> p[i].d >> p[i].l;
    }
    if (dfs(0, 0))
        cout << "YES" << endl;
    else
        cout << "NO" << endl;
    // 重新刷新数据，准备第二组数据
    for (int i = 0; i < n; i++)
    {
        st[i] = false;
    }
}
int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    int t = 1;
    cin >> t;
    while (t--)
        solve();
    return 0;
}
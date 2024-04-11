// https://www.lanqiao.cn/problems/3503/learning/?subject_code=1&group_code=5&match_num=14&match_flow=1&origin=cup
#include <bits/stdc++.h>
#define endl '\n'
#define deb(x) cout << #x << " = " << x << '\n';
#define INF 0x3f3f3f3f
using namespace std;
const int N = 5e3 + 10;
int f[N][N];

void solve()
{
    string s;
    cin >> s;
    int ans = 0;
    for (int len = 2; len <= s.size(); len++) // 从子串长度为2的情况开始枚举，为1和0必然不成立
    {
        for (int l = 0; l + len - 1 < s.size(); l++) // 右端点 < 字串长
        {
            int r = l + len - 1; // 右端点r = 左端点l + 字串长度len - 1

            if (s[l] > s[r]) // 状态判断：首尾大于末尾，翻转后变小
                f[l][r] = 1;
            else if (s[l] == s[r]) // 状态转移：如果字串首末位大小相等，将翻转是否成功的判断转移给l+1位和r-1位（第二和倒数第二）
                f[l][r] = f[l + 1][r - 1];

            ans += f[l][r];
        }
    }
    cout << ans << endl;
}
signed main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    int t;
    t = 1;
    // cin >> t;
    while (t--)
        solve();
}

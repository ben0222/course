// https://www.lanqiao.cn/problems/3519/learning/?subject_code=1&group_code=3&match_num=14&match_flow=1&origin=cup
#include <bits/stdc++.h>
using namespace std;
#define int long long
#define endl '\n'
#define deb(x) cout << #x << '=' << x << '\n';
#define INF 0x3f3f3f3f
const int N = 100010;

void solve()
{
    string str;
    cin >> str;
    int count = 0;
    vector<bool> st(str.size()); // 记录i位置是否匹配过
    for (int i = 0; i < str.size(); i++)
    {
        if (str[i] != '?')
        {
            if (st[i] == false)
            {
                if (str[i - 1] == str[i] && i - 1 >= 0 && st[i - 1] == false)
                {
                    st[i - 1] = st[i] = true;
                    count++;
                }
                else if (str[i] == str[i + 1] && i + 1 < str.size() && st[i + 1] == false)
                {
                    st[i] = st[i + 1] = true;
                    count++;
                }
            }
        }
        else // 如果当前字符是问号，则检查其相邻字符是否已经被匹配过，如果没有，则将当前字符和相邻字符标记为匹配，并增加答案计数。
        {
            if (st[i] == true)
                continue; // st[i]经历了下面的操作后会配对成功，则遍历到st[i+1]时候，需要识别到st[i]是识别过的。
            if (st[i - 1] == false && i - 1 >= 0)
            {
                st[i - 1] = st[i] = true;
                count++;
            }
            else if (st[i + 1] == false && i + 1 < str.size())
            {
                st[i] = st[i + 1] = true;
                count++;
            }
        }
    }
    cout << count << endl;
}
signed main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    int t = 1;
    while (t--)
        solve();
    return 0;
}
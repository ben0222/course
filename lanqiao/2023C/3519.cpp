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
        if (st[i] != '?')
        {
            if (st[i] == false)
            {
                if (st[i - 1] == st[i] && i - 1 >= 0 && st[i - 1] == false)
                {
                    st[i - 1] = st[i] = true;
                    count++;
                }
                else if (st[i] == st[i + 1] && i + 1 < st.size() && st[i + 1] == false)
                {
                    st[i] = st[i + 1] = true;
                    count++;
                }
            }
        }
        else
        {
            if (st[i] == true)
                continue; // 已经被配对了，下一个
            if (st[i - 1] == true && i - 1 >= 0)
            {
                st[i - 1] = st[i] = true;
                count++;
            }
            else if (st[i + 1] == true && i + 1 < str.size())
            {
                st[i] = st[i + 1] = true;
                count++;
            }
        }
    }
    cout << count << endl;
}
unsigned main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    int t = 1;
    while (t--)
        solve();
}
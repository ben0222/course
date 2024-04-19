#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const int N = 3e6;
int n1, n2;
int a[N], b[N];
void solve()
{
    cin >> n1;
    for (int i = 0; i < n1; i++)
    {
        cin >> a[i];
    }
    cin >> n2;
    for (int i = 0; i < n2; i++)
    {
        cin >> b[i];
    }
    // 归并排序
    int index = 0, end = (n1 + n2 + 1) / 2;
    int i = 0, j = 0;
    int cur1 = 0, cur2 = 0;
    int flag = 0; // 标记元素来自哪一个数组
    while (index != end)
    {
        if (i < n1 && a[i] < b[j])
        {
            cur1 = i++;
            flag = 1;
        }
        else if (j < n2)
        {
            cur2 = j++;
            flag = 2;
        }
        index++;
    }
    if (flag == 1)
    {
        cout << a[cur1];
    }
    else
    {
        cout << b[cur2];
    }
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
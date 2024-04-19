#include <bits/stdc++.h>
using namespace std;
const int N = 100010;
int coordinator[N];
void solve()
{
    int n = 0;
    ;
    cin >> n;
    for (int i = 0; i < n; i++)
    {
        cin >> coordinator[i];
    }
    // 将商店的坐标排序
    sort(coordinator, coordinator + n);

    // 计算最优的货仓位置
    int warehouse;
    if (n % 2 == 1)
    {
        warehouse = coordinator[n / 2];
    }
    else
    {
        warehouse = (coordinator[n / 2 - 1] + coordinator[n / 2]) / 2;
    }

    // 计算货仓到每家商店的距离之和
    long long total_distance = 0;
    for (int i = 0; i < n; i++)
    {
        total_distance += abs(coordinator[i] - warehouse);
    }

    cout << total_distance << endl;
}

int main()
{
    solve();
    return 0;
}
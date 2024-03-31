// https://www.lanqiao.cn/problems/3517/learning/?subject_code=1&group_code=4&match_num=14&match_flow=1&origin=cup
#include <bits/stdc++.h>
#include <iostream>
using namespace std;
const int N = 1e5 + 10;
typedef pair<int, int> p;

vector<int> edge[N]; // 图存储：存这个点的邻接点,为每个点开辟一个vector
int n, m;
int weight[N];  // 存边的权值
map<p, int> id; // 存边的编号

bool dfs(int s, int u, int father, int v) // 起点s，终点v，当前点u，当前点的父节点father
{
    if (u == v)
        return true;

    for (int i = 0; i < edge[u].size(); i++) // 遍历当前点u的所有子节点
    {
        int son = edge[u][i];
        if (father == son)
            continue;          // 因为在这种图的存储方式下，一个点的子节点也包括它的父节点，所以当遍历到父节点的时候要直接进入下一次循环
        if (dfs(s, son, u, v)) // dfs逻辑：对每一个son进行dfs，则当前点为son，当前点的父节点为u
        {
            int ID = id[{u, son}];
            weight[ID]++;
            return true;
        }
    }
    return false;
}
void solve()
{
    // 图初始化
    cin >> n >> m;
    for (int i = 0; i < n - 1; i++)
    {
        int x, y;
        cin >> x >> y;
        // 无向边：存两次，等价于两条有向边
        edge[x].push_back(y);
        edge[y].push_back(x);
        id[{x, y}] = id[{y, x}] = i;
    }
    for (int i = 0; i < m; i++)
    {
        int x, y;
        cin >> x >> y;
        dfs(x, x, -1, y);
    }
    int ans = -1;
    for (int i = n - 1; i >= 0; i--) // 倒序枚举，第一次输出的就是最编号最大的边
    {
        if (weight[i] == m)
        {
            ans = i + 1;
            break;
        }
    }
    cout << ans << endl;
}
int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    return 0;
}
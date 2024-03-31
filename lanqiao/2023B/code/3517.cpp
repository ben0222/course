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
            continue;
        if (dfs(s, son, u, v))
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
}
int main()
{
    return 0;
}
// 砍树：树上差分 + 最近公共祖先
#include <bits/stdc++.h>
using namespace std;
typedef pair<int, int> pii;
const int N = 1e5 + 10;
int n, m;
int w[N]; // 记录每个点的点权。

map<pii, int> id;
vector<int> edge[N];

int siz[N], dep[N], fa[N], son[N], top[N]; // 节点大小、深度、父节点、重儿子、所在链顶端

// DFS遍历，计算节点大小、深度、重儿子
void dfs1(int u, int father)
{
    siz[u] = 1;               // 初始化节点大小为1
    dep[u] = dep[father] + 1; // 计算节点深度
    fa[u] = father;           // 记录父节点
    for (int i = 0; i < edge[u].size(); i++)
    {
        int s = edge[u][i];
        if (s == fa[u]) // 如果相邻节点是父节点，则跳过
            continue;
        dfs1(s, u);               // 递归遍历子节点
        siz[u] += siz[s];         // 更新节点大小
        if (siz[son[u]] < siz[s]) // 更新重儿子
            son[u] = s;
    }
}

// DFS遍历，确定每个节点所在链的顶端
void dfs2(int u, int t)
{
    top[u] = t;      // 记录节点所在链的顶端
    if (son[u] == 0) // 如果没有重儿子，则返回
        return;
    dfs2(son[u], t); // 继续遍历重儿子所在的链
    for (int i = 0; i < edge[u].size(); i++)
    {
        int s = edge[u][i];
        if (s == fa[u] || s == son[u]) // 跳过父节点和重儿子
            continue;
        dfs2(s, s); // 遍历非重儿子所在的链
    }
}

// 寻找两个节点的最近公共祖先
int lca(int x, int y)
{
    while (top[x] != top[y])
    {                                  // 如果两个节点不在同一条链上
        if (dep[top[x]] < dep[top[y]]) // 如果x所在链深度较小，则交换x和y
            swap(x, y);
        x = fa[top[x]]; // 将x移动到所在链的顶端的父节点
    }
    return dep[x] < dep[y] ? x : y; // 返回最近公共祖先
}

void cal_sum(int u, int father)
{
    for (int i = 0; i < edge[u].size(); i++)
    {
        int son = edge[u][i];
        if (son == father)
            continue;
        cal_sum(son, u);
        w[u] += w[son];
    }
}

void solve()
{
    cin >> n >> m;
    for (int i = 1; i <= n - 1; i++)
    {
        int x, y;
        cin >> x >> y;
        edge[x].push_back(y);
        edge[y].push_back(x);
        id[{x, y}] = i;
        id[{y, x}] = i;
    }

    dfs1(1, 0);
    dfs2(1, 1);

    for (int i = 0; i < m; i++)
    {
        int a, b;
        cin >> a >> b;

        // 做树上差分
        w[a]++, w[b]++;
        w[lca(a, b)] -= 2;
    }

    cal_sum(1, 0);

    int ans = -1;
    for (int i = 1; i <= n; i++)
    {
        if (w[i] == m)
        {
            int ID = id[{i, fa[i]}];
            ans = max(ans, ID);
        }
    }
    cout << ans << endl;
}

signed main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    int t = 1;
    // cin >> t;
    while (t--)
        solve();
}

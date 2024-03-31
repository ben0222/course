// 砍树：树上差分 + 最近公共祖先
#include <bits/stdc++.h>
using namespace std;
typedef pair<int, int> pii;
const int N = 1e5 + 10;
int n, m;
int w[N]; // 记录每个点的点权。

map<pii, int> id;
vector<int> edge[N];

int siz[N], dep[N], fa[N], son[N], top[N];

void dfs1(int u, int father)
{
    siz[u] = 1, dep[u] = dep[father] + 1;
    fa[u] = father;
    for (int i = 0; i < edge[u].size(); i++)
    {
        int s = edge[u][i];
        if (s == fa[u])
            continue;
        dfs1(s, u);
        siz[u] += siz[s];
        if (siz[son[u]] < siz[s])
            son[u] = s;
    }
}

void dfs2(int u, int t)
{
    top[u] = t;
    if (son[u] == 0)
        return;
    dfs2(son[u], t);
    for (int i = 0; i < edge[u].size(); i++)
    {
        int s = edge[u][i];
        if (s == fa[u] || s == son[u])
            continue;
        dfs2(s, s);
    }
}

int lca(int x, int y)
{
    while (top[x] != top[y])
    {
        if (dep[top[x]] < dep[top[y]])
            swap(x, y);

        x = fa[top[x]];
    }
    return dep[x] < dep[y] ? x : y;
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

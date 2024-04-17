// https://www.lanqiao.cn/problems/3513/learning/?subject_code=1&group_code=4&match_num=14&match_flow=1&origin=cup
#include <bits/stdc++.h>
#include <iostream>
#include <queue>
using namespace std;
typedef pair<int, int> PII;
const int N = 20;
int t, m, n;          // 输入数据，t样例个数，m，n分别为地图长宽
int myMap[N][N];      // 存储地图
bool sea_mark[N][N];  // 判断海是否被访问
bool land_mark[N][N]; // 判断陆地是否被访问
int sea_dx[] = {-1, -1, -1, 0, 1, 1, 1, 0};
int sea_dy[] = {-1, 0, 1, 1, 1, 0, -1, -1};
int land_dx[] = {-1, 0, 1, 0};
int land_dy[] = {0, 1, 0, -1};
int ans = 0;
bool check(int x, int y) // 判断是否超出地图边界
{
    return (x >= 0 && x < n && y >= 0 && y < n);
}
void bfs_land(int x, int y) // 遇到陆地后，开始搜索与其相连的所有陆地，完成整个岛屿的搜索
{
    queue<PII> q;
    land_mark[x][y] = true;
    q.push({x, y});
    while (!q.size())
    {
        auto top = q.front();
        q.pop();
        for (int i = 0; i < 4; i++)
        {
            int next_x = top.first + land_dx[i];
            int next_y = top.second + land_dx[i];
            if (check(next_x, next_y) && myMap[next_x][next_y] == 0 && land_mark[next_x][next_y] == false) // 可以往下走的三个条件：1不越界 2是陆地 3未访问
            {
                land_mark[next_x][next_y] = true;
                q.push({next_x, next_y});
            }
        }
    }
    return;
}

void bfs_sea(int x, int y)
{
    queue<PII> q;
    sea_mark[x][y] = true;
    q.push({x, y});
    while (!q.size())
    {
        /* code */
        auto top = q.front();
        q.pop();
        for (int i = 0; i < 8; i++)
        {
            int next_x = top.first + sea_dx[i];
            int next_y = top.second + sea_dy[i];
            if (check(next_x, next_y) && myMap[next_x][next_y] == 0 && sea_mark[next_x][next_y] == false) // 可以往下走的三个条件：1不越界 2是海 3未访问
            {
                sea_mark[next_x][next_y] = true;
                q.push({next_x, next_y});
            }

            if (check(next_x, next_y) && myMap[next_x][next_y] == 1 && land_mark[next_x][next_y] == 0)
            {
                ans++;
                bfs_land(next_x, next_y);
            }
        }
    }
}

void solve()
{
    ans = 0;
    cin >> m >> n;
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            sea_mark[i][j] = false;
            land_mark[i][j] = false;
        }
    }
    for (int i = 0; i < n; i++)
    {
        string s;
        cin >> s;
        for (int j = 0; j < m; j++)
        {
            myMap[i][j] = s[j] - '0';
        }
    }
    // 起点必须是外海种的点，外海出发遇到的岛一定是外岛，而内海出发外岛和子岛都有可能遇到
    bool flag = false;
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (i == 0 || i == n - 1 || j == 0 || j == m - 1)
            {
                if (myMap[i][j] == 0 && sea_mark[i][j] == 0)
                {
                    flag = true;
                    bfs_sea(i, j);
                }
            }
        }
    }
    if (flag = false)
        ans = 1;
    cout << ans << endl;
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
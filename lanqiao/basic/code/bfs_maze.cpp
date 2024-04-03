#include <iostream>
#include <cstring>
#include <queue>
using namespace std;
const int N = 110;
typedef pair<int, int> PII;
int map[N][N], mark[N][N];
int dx[4] = {-1, 0, 1, 0};
int dy[4] = {0, 1, 0, -1};
int n, m, ans;
void bfs()
{
    memset(mark, -1, sizeof(mark));
    queue<PII> q;
    q.push({0, 0});
    mark[0][0] = 0;
    while (!q.empty())
    {
        PII top = q.front();
        for (int i = 0; i < 4; i++)
        {
            int next_x = top.first + dx[i];
            int next_y = top.second + dy[i];
            if (next_x >= 0 && next_x < n && next_y >= 0 && next_y < n && mark[next_x][next_y == -1 && map[next_x][next_y] == 0)
            {
                mark[next_x][next_y] = mark[top.first][top.second] + 1;
                q.push({next_x, next_y});
            }
        }
    }
    cout << mark[n - 1][m - 1];
}
int main()
{
    cin >> n >> m;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cin >> map[i][j];
        }
    }
    bfs();
    return 0;
}
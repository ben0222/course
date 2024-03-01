#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main()
{
    int n;
    cin >> n;

    vector<vector<int>> grid(n, vector<int>(n));
    vector<vector<int>> dp(n, vector<int>(n, 0));

    // 输入金币数量
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            cin >> grid[i][j];
        }
    }

    // 动态规划，计算每个位置上的最大金币数量
    dp[0][0] = grid[0][0];
    for (int i = 1; i < n; ++i)
    {
        dp[i][0] = dp[i - 1][0] + grid[i][0]; // 第一列只能从上面一格移动
        dp[0][i] = dp[0][i - 1] + grid[0][i]; // 第一行只能从左边一格移动
    }

    for (int i = 1; i < n; ++i)
    {
        for (int j = 1; j < n; ++j)
        {
            dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]) + grid[i][j];
        }
    }

    // 输出最大金币数量
    cout << dp[n - 1][n - 1] << endl;

    return 0;
}

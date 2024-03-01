#include <iostream>
#include <vector>
#include <iomanip>
#include <math.h>

int main()
{
    using namespace std;

    int n, m;
    cin >> n >> m;

    vector<vector<double>> dp(n + 1, vector<double>(m + 1, 0.0));
    dp[0][0] = 1.0;

    for (int i = 1; i <= m; ++i)
    {
        for (int j = 1; j <= n; ++j)
        {
            if (i < j)
                dp[i][j] = 0; // 购买的印章张数小于出现的种类数，此时概率为0
            if (j == 1)
                dp[i][j] = pow(1.0 / n, i - 1); // 只出现一种印章的概率
            else
                // dp[j][i]表示：在第i次购买后，集齐了j种邮票的概率
                // 这个概率分两种情况：一是在i-1次购买就集齐了j种，且在第i次又买到了这j种的其中一个
                // 二是在i-1次购买只集齐了j-1种，在第i次刚好集齐j种
                dp[j][i] = dp[j][i - 1] * (j / n) + dp[j - 1][i - 1] * (n - (j - 1) / n);
        }
    }
    // 题目要求的是在购买了m张印章后，恰好集齐了n种印章的概率。
    // 因此，我们需要考虑购买了m-1张印章后已经集齐了n种印章的情况，然后在第m次购买中成功集齐第n种印章的概率。
    cout << fixed << setprecision(4) << 1.0 - dp[n][m - 1] << endl;

    return 0;
}

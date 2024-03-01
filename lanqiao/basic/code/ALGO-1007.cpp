#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

int main()
{
    int n, m;
    cin >> n >> m;
    // 我们可以定义一个动态规划数组 dp，其中 dp[i] 表示购买 i 张印章时集齐 n 种印章的概率。
    vector<double> dp(m + 1, 0.0);
    // 其中，dp[i-1] 表示购买 i-1 张印章时已经集齐 n 种印章的概率，而购买第 i 张印章时，成功集齐剩余的印章的概率为1 - 1/n
    // 最终，我们可以得到购买 m 张印章时集齐 n 种印章的概率为 dp[m]。
    dp[0] = 1.0;
    for (int i = 1; i <= m; i++)
    {
        dp[i] = dp[i - 1] * (1.0 - 1.0 / n);
    }
    cout << fixed << setprecision(4) << dp[m] << endl;

    return 0;
}
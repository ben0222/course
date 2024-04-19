#include <bits/stdc++.h>
using namespace std;
int ans = 0;
int count(vector<int> v)
{
    int count = 0;
    for (int i = 0; i < v.size(); i++)
    {
        for (int j = i + 1; j < v.size(); j++)
        {
            if (v[i] < v[j])
                count++;
        }
    }
    return count;
}
int main()
{
    int n, k;
    cin >> n >> k;
    vector<int> a;
    vector<int> fuzzy;
    int num;
    for (int i = 0; i < n; i++)
    {
        cin >> num;
        a.push_back(num);
    }
    for (int i = 1; i <= n; i++)
    {
        if (find(a.begin(), a.end(), i) == a.end())
        {
            fuzzy.push_back(i);
        }
    }
    do
    {
        vector<int> a_copy(a);
        int cnt = 0;
        for (int i = 0; i < fuzzy.size(); i++)
        {
            while (1)
            {
                if (a_copy[cnt] == 0)
                {
                    a_copy[cnt] = fuzzy[i]; // 将所有的排列组合放入放入原数组，看顺序对是否相等 相等即为一种可能性
                    break;
                }
                cnt++;
            }
        }
        if (count(a_copy) == k)
            ans++;
    } while (next_permutation(fuzzy.begin(), fuzzy.end()));
    cout << ans << endl;
    return 0;
}
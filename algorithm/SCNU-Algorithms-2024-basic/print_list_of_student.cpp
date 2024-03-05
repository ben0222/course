// #include <iostream>
// #include <vector>
// using namespace std;
// const int N = 40000;
// const int K = 2500;
// const int C = 20;
// int main()
// {
//     // 二维字符串向量，构建一个二维矩阵来存放学生列表
//     vector<vector<string>> list;
//     int n, k, c;
//     cin >> n >> k;
//     for (int i = 0; i < n; i++)
//     {
//         string name;
//         int num_course;
//         cin >> name >> num_course;
//         cout << name << num_course << endl;
//         string num_course_str = to_string(num_course);
//         cout << num_course_str << endl;
//         // 确定了第i个学生的课程数量，就能确定该学生信息需要的一维字符串数组大小
//         // 其大小为2+课程数量（名字+课程数+课程编号1+课程编号2+...）
//         list[i].resize(1 + 1 + num_course);
//         list[i].push_back(name);
//         list[i].push_back(num_course_str);

//         for (int j = 0; j < num_course; j++)
//         {
//             string id_course;
//             cin >> id_course;
//             list[i].push_back(id_course);
//         }
//     }
//     vector<vector<string>>::iterator i;
//     vector<string>::iterator j;
//     for (i = list.begin(); i != list.end(); i++)
//     {
//         for (j = i->begin(); j != i->end(); j++)
//         {
//             cout << *j << endl;
//         }
//     }

//     return 0;
// }
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const int N = 2510;
vector<string> v[N];
void solve()
{
    int n, k;
    cin >> n >> k;
    for (int i = 1; i <= n; i++)
    {
        string name;
        int num_course;
        cin >> name >> num_course;
        while (num_course--)
        {
            int no_course;
            cin >> no_course;
            v[no_course].push_back(name);
        }
    }
    for (int i = 1; i <= k; i++)
    {
        sort(v[i].begin(), v[i].end());
        cout << i << " " << v[i].size() << '\n';
        for (auto name : v[i])
        {
            cout << name << '\n';
        }
    }
}
int main()
{
    ios::sync_with_stdio(0), cin.tie(0), cout.tie(0);
    int t = 1;
    // cin >> t;
    while (t--)
        solve();
    return 0;
}
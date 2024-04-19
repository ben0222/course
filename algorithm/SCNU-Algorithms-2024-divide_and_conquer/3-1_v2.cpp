#include <iostream>
#include <vector>
#include <algorithm> // 用于排序

using namespace std;

int main()
{
    int N;
    cin >> N; // 读取整数 N，表示数组的大小

    vector<long long> nums(N); // 声明一个大小为 N 的向量来保存输入的整数

    // 依次读取 N 个整数并保存到向量 nums 中
    for (int i = 0; i < N; ++i)
    {
        cin >> nums[i];
    }

    // 使用标准库中的 sort 函数对向量 nums 进行排序
    sort(nums.begin(), nums.end());

    // 输出排序后的结果
    for (int i = 0; i < N; ++i)
    {
        cout << nums[i];
        if (i != N)
            cout << ' ';
    }

    return 0;
}

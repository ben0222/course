#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void classifyPeople(int N, vector<int> &activities)
{
    sort(activities.begin(), activities.end(), greater<int>()); // 按活跃度从高到低排序
    int outgoingCount = 0;
    int introvertedCount = 0;
    int outgoingSum = 0;
    int introvertedSum = 0;

    int left = 0;
    int right = N - 1;

    while (left <= right)
    {
        if (outgoingCount <= introvertedCount)
        {
            outgoingSum += activities[left];
            outgoingCount++;
            left++;
        }
        else
        {
            introvertedSum += activities[right];
            introvertedCount++;
            right--;
        }
    }

    // 计算两类人群的总活跃度之差
    int diff = abs(outgoingSum - introvertedSum);

    // 输出结果
    cout << "Outgoing #: " << outgoingCount << endl;
    cout << "Introverted #: " << introvertedCount << endl;
    cout << "Diff = " << diff << endl;
}

int main()
{
    int N;
    cin >> N;
    vector<int> activities(N);

    for (int i = 0; i < N; ++i)
    {
        cin >> activities[i];
    }

    classifyPeople(N, activities);

    return 0;
}

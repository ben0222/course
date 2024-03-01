#include <iostream>
#include <vector>
using namespace std;
// int bubbleSort(int nums[],int len)
// {
//   int count;
//   for(int i = len - 1; i>0;i--)
//   {
//     for(int j = 0;j<i;j++)
//     {
//       int temp = nums[j];
//       nums[j] = nums[j+1];
//       nums[j+1] = temp;
//       count += 3;
//     }
//   }
//   return count;
// }
int bubbleSort(vector<int> &nums) {
    int count = 0; // 计数器
    // 外循环：未排序区间为 [0, i]
    for (int i = nums.size() - 1; i > 0; i--) {
        // 内循环：将未排序区间 [0, i] 中的最大元素交换至该区间的最右端 
        for (int j = 0; j < i; j++) {
            if (nums[j] > nums[j + 1]) {
                // 交换 nums[j] 与 nums[j + 1]
                int tmp = nums[j];
                nums[j] = nums[j + 1];
                nums[j + 1] = tmp;
                count += 3; // 元素交换包含 3 个单元操作
            }
        }
    }
    return count;
}
int exponential(int n)
{
  int count = 0;
  int base = 1;
  for(int i = 0;i < n;i++)
  {
    for(int j = 0;j < base;j++)
    {
      count++;
    }
    base *= 2;
  }
  return count;
}
int expRecur(int n)
{
  if(n == 1)
  {
    return 1;
  }
  return  2 * expRecur(n - 1) + 1;
}
  int main()
{
  vector<int> a={3,4,2,6,8};
  cout << bubbleSort(a) << endl;
  for(int i = 0;i < 10;i++)
  {
    cout << exponential(i) << endl;
  }
  
  return 0;
}
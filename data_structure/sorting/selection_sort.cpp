#include <iostream>
using namespace std;

void selectionSort(int arr[], int n)
{
    for(int i = 0; i < n - 1; i++)
    {
        int minIndex = i;
        for(int j = i + 1; j < n; j++)//寻找未排序序列中的最小值，并将其下表保存在minIndex
        {   
            if(arr[j] < arr[minIndex])
            {
                minIndex = j;
            }
            
        } 
        swap(arr[i], arr[minIndex]);     //交换：本轮处理的值arr[i]、未排序序列中的最小值
    }
}
int main()
{
    int arr[] = {6, 2, 1, 3, 4, 5, 7};
    int len = sizeof(arr) / sizeof(arr[0]);

    selectionSort(arr, len);
    
    for(int i = 0; i < len; i++)
    {
        cout << arr[i] << " ";
    }
    cout << endl;
    return 0;
}
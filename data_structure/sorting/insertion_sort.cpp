#include <iostream>
using namespace std;
//直接插入排序
void insertionSort(int arr[], int n)
{
    for(int i = 1; i < n; i++)
    {
        int key = arr[i];
        int j = i- 1;

        //大于key的元素向后移动
        while(j >= 0 && arr[j] > key)
        {
            swap(arr[j + 1], arr[j]);
            j--;
        }
    }
}
int main()
{
    int arr[] = {6, 2, 1, 3, 4, 5, 7};
    int len = sizeof(arr) / sizeof(arr[0]);

    insertionSort(arr, len);
    
    for(int i = 0; i < len; i++)
    {
        cout << arr[i] << " ";
    }
    cout << endl;
    return 0;
}
#include <iostream>
using namespace std;

//在 heapify 函数中，给定一个数组和一个索引 i，它会调整以 i 为根的子树，使其满足最大堆性质。
//它首先找到左子节点和右子节点中较大的那个，然后将较大的子节点的值与根节点的值进行交换，再递归地对被交换的子节点进行堆调整。
void heapify(int arr[], int n, int i)
{
    int largest = i;//初始化最大元素为根节点
    //因为堆是完全二叉树，用数组储存的方法下，某结点与其父节点，左子右子在数组下标处存在数学关系：
    //若某节点存在下标i，则其父节点存在(i - 1)/2,左子在 2i + 1,右子在2i - 1
    //称之为索引映射
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if(left < n && arr[left] > arr[largest])
        largest = left;

    if(right < n && arr[right] > arr[largest])
        largest = right;

    //若上面两步(即检查左右节点有没有谁比根节点大)检查出来了(largest发生交换了，不等于i了)，则进行子与根的交换
    if(largest != i)
    {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }

}
//在 heapSort 函数中，首先建立初始的最大堆，然后将最大堆的根节点（最大值）与数组的最后一个元素交换，再调整堆，
//重复这个过程直到堆的大小为1。最终，数组就被排序了。
void heapSort(int arr[], int n)
{
    for(int i = n / 2 - 1; i >= 0; i--)
    {
        heapify(arr, n ,i);
    }

    // 一个个从堆顶取出元素
    for(int i = n - 1;i > 0; i--)
    {
         // 将当前最大元素移到数组末尾
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}
int main()
{
    int arr[] = {6, 2, 1, 3, 4, 5, 7};
    int len = sizeof(arr) / sizeof(arr[0]);

    heapSort(arr, len);
    
    for(int i = 0; i < len; i++)
    {
        cout << arr[i] << " ";
    }
    cout << endl;
    return 0;
}
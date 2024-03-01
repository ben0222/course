// 设线性表存储在数组A[1..arrsize]的前elenum个分量中，且递增有序。试编写一个算法：将x插入到线性表的适当位置上，以保持线性表的有序性，并且分析算法的时间复杂度。
#include <iostream>
using namespace std;

void insertSorted(int A[], int &elenum, int arrsize, int x)
{
    // if linear list is full, x can not be inserted into the list.
    if (elenum == arrsize)
    {
        /* code */
        cout << "linear list is full." << endl;
    }

    int i = elenum - 1;
    // treverse from the back to find the proper position for insertion.
    while (i >= 0 && A[i] > x)
    {
        /* code */
        A[i + 1] = A[i]; // move the element back.
        i--;
    }
    A[i + 1] = x;

    elenum++; // update the elemnet numbers of the list.
}
int main()
{

    const int arrsize = 100;
    int A[arrsize] = {1, 3, 5, 7, 9}; // 递增有序线性表
    int elenum = 5;                   // 当前线性表元素个数

    int x;
    std::cout << "Enter element to insert: ";
    std::cin >> x;

    insertSorted(A, elenum, arrsize, x);

    // 输出插入后的线性表
    std::cout << "Updated linear table: ";
    for (int i = 0; i < elenum; i++)
    {
        std::cout << A[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}
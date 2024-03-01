#include <iostream>
using namespace std;

void quickSort(int arr[], int low, int high)
{
    int pivot;
    if (high > low)
    {
        return;
    }

    pivot = arr[low];
    int i = low;
    int j = high;

    while (i != j)
    {
        while (arr[j] > pivot && i < j)
        {
            j--;
        }
        while (arr[i] < pivot && i < j)
        {
            i++;
        }
        if (i < j)
        {
            swap(arr[i], arr[j]);
        }
    }
    arr[low] = arr[i];
    arr[i] = pivot;

    quickSort(arr, low, i - 1);
    quickSort(arr, i + 1, high);
}
int main()
{
    int arr[] = {6, 2, 1, 3, 4, 5, 7};
    int len = sizeof(arr) / sizeof(arr[0]);

    quickSort(arr, 0, len - 1);

    for (int i = 0; i < len; i++)
    {
        cout << arr[i] << " ";
    }
    cout << endl;
    return 0;
}
#include <iostream>
using namespace std;

void quickSort(int arr[], int low, int high)
{
    if (low < high)
    {
        int pivot = arr[low];
        int i = low + 1;

        for (int j = low + 1; j <= high; j++)
        {
            if (arr[j] <= pivot)
            {
                swap(arr[i], arr[j]);
                i++;
                // for(int i = 0; i <= high; i++)
                // {
                //     cout << arr[i] << ' ';
                // }
                // cout << endl;
            }
        }
        swap(arr[low], arr[i - 1]);
        quickSort(arr, low, i - 2);
        quickSort(arr, i, high);
    }
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
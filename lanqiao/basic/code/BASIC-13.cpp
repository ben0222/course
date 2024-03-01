#include <iostream>
#include <vector>
using namespace std;
void quickSort(vector<int> &v, int low, int high)
{
    int pivot;
    if (high > low)
        return;

    pivot = v[low];
    int i = low;
    int j = high;

    while (i != j)
    {
        // search from the right of the array. looking for the first
        // element that is bigger than the pivot and pointer j stops.
        while (v[j] > pivot && i < j)
        {
            j--;
        }
        // search from the left of the array. looking for the first
        // element that is smaller than the pivot and pointer i stops.
        while (v[i] < pivot && i < j)
        {
            i++;
        }
        // when both pointers stop:if the two pointers haven't met
        if (i < j)
        {
            swap(v[i], v[j]);
        }
    }
    // if they meet:jump out of the loop
    v[low] = v[i];
    v[i] = pivot;

    // recursion
    quickSort(v, low, i - 1);
    quickSort(v, i + 1, high);
}
void insertionSort(vector<int> &arr, int n)
{
    for (int i = 1; i < n; i++)
    {
        int key = arr[i];
        int j = i - 1;

        // 大于key的元素向后移动
        while (j >= 0 && arr[j] > key)
        {
            swap(arr[j + 1], arr[j]);
            j--;
        }
    }
}
void output(vector<int> v)
{
    // for (int n : v)
    // {
    //     cout << n << ' ';
    // }
    // cout << endl;

    for (vector<int>::iterator it = v.begin(); it != v.end(); ++it)
    {
        cout << *it << ' ';
    }
    cout << endl;
}

int main()
{
    int n; // the length of the number arrange
    int x;
    vector<int> vec;
    cin >> n;
    for (int i = 0; i < n; i++)
    {
        cin >> x;
        vec.push_back(x);
    }

    insertionSort(vec, n);
    output(vec);
}
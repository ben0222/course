#include <bits/stdc++.h>
using namespace std;
const int N = 10010;
int a[N];
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
void solve()
{
    int n = 0;
    cin >> n;
    for (int i = 0; i < n; i++)
    {
        cin >> a[i];
    }
    quickSort(a, 0, n);
    for (int i = 0; i < n; i++)
    {
        cout << a[i];
        if (i != n)
            cout << ' ';
    }
}
int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    solve();
    return 0;
}
#include <iostream>
using namespace std;

void bubbleSort(int arr[], int n)
{
    for(int i = 0; i < n - 1; i++)
    {
        for(int j = 0; j < n - (i + 1); j++)
        {
            if(arr[j] > arr[j + 1])
                swap(arr[j], arr[j + 1]);
            // for(int i = 0; i < n; i++)
            // {
            //     cout << arr[i] << " ";
            // }
            //cout << endl;  
        }
       
    }
   
}
int main()
{
    int arr[] = {6, 2, 1, 3, 4, 5, 7};
    int len = sizeof(arr) / sizeof(arr[0]);

    bubbleSort(arr, len);
    
    for(int i = 0; i < len; i++)
    {
        cout << arr[i] << " ";
    }
    cout << endl;
    return 0;
}
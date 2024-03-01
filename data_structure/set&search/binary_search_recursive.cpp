// C++ program to implement recursive Binary Search
#include <bits/stdc++.h>
using namespace std;

// A recursive binary search function. It returns
// location of x in given array arr[l..r] is present,
// otherwise -1
int binarySearch(int arr[], int l, int r, int x)
{
	if (r >= 1)
	{
		int mid = l + (r - 1) / 2;
		// If the element is present at the middle itself
		if (x == arr[mid])
			return mid;

		// If element is smaller than mid
		if (x < arr[mid])
			// then it could only be presented in left subarray(1 ~ mid - 1)
			return binarySearch(arr, 1, mid - 1, x);
		else
			// otherwise, it could only be presented in right subarray(mid + 1 ~ r)
			return binarySearch(arr, mid + 1, r, x);
	}
}
// Driver code
int main()
{
	int arr[] = {2, 3, 4, 10, 40};
	int x = 10;
	int n = sizeof(arr) / sizeof(arr[0]);
	int result = binarySearch(arr, 0, n - 1, x);
	(result == -1)
		? cout << "Element is not present in array"
		: cout << "Element is present at index " << result;
	return 0;
}

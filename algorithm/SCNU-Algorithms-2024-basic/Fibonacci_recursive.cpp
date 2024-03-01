#include <iostream>
using namespace std;
int fibonacci_recursive(int n)
{
    if (n == 1 || n == 2)
        return 1;
    return fibonacci_recursive(n - 1) + fibonacci_recursive(n - 2);
}
int main()
{
    int n = 0;
    cin >> n;
    cout << fibonacci_recursive(n) << endl;
    return 0;
}
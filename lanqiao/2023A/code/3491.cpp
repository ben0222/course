#include <iostream>
using namespace std;
void getDigit_4(int digit[], int x)
{
    for (int i = 3; i >= 0; i--)
    {
        digit[i] = x % 10;
        x /= 10;
    }
}
void getDigit_6(int digit[], int x)
{
    for (int i = 5; i >= 0; i--)
    {
        digit[i] = x % 10;
        x /= 10;
    }
}
void getDigit_8(long long int digit[], int x)
{
    for (int i = 7; i >= 0; i--)
    {
        digit[i] = x % 10;
        x /= 10;
    }
}
int main()
{
    int count = 0;
    int digit_2[1];           // 9
    int digit_4[3];           // 615
    int digit_6[5];           // 50412
    long long int digit_8[7]; // 4379055

    for (long long int i = 10000000; i < 100000000; i++)
    {
        getDigit_8(digit_8, i);
        if (digit_8[0] + digit_8[1] + digit_8[2] + digit_8[3] == digit_8[4] + digit_8[5] + digit_8[6] + digit_8[7])
            count++;
    }
    cout << count << endl;
    return 0;
}
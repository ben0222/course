#include <iostream>
#include <vector>
using namespace std;

int main()
{
    int n;
    vector<vector<int>> cell;
    // 分配内存
    cell.resize(n, vector<int>(n));
    int count;
    int x;
    cin >> n;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cin >> x;
            cell[i][j] = x;
        }
    }

    count = cell[0][0];

    int i = 0, j = 0;
    while(i < n && j < n)
    {
        if(i == n)j++;
        if(j == n)i++;
        if(cell[i][j + 1] > cell[i + 1][j])
        {
            j++;
            count += cell[i][j + 1];
        }
        if(cell[i][j + 1] < cell[i + 1][j])
        {
            i++;
            count += cell[i + 1][j];
        }
    }
    cout << count << endl;

    // for (int i = 0; i < n; i++)
    // {
    //     for (int j = 0; j < n; j++)
    //     {

    //         cout << cell[i][j] << ' ';
    //     }
    //     cout << endl;
    // }
    return 0;
}
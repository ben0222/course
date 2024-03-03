#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
struct edge
{
    int village1;
    int village2;
    int cost;
};

bool compare(const edge &a, const edge &b)
{
    return a.cost < b.cost;
}

class UnionFind
{
public:
    UnionFind(int n) : parent(n), rank(n, 0)
    {
        for (int i = 0; i < n; i++)
            parent[i] = i;
    }

private:
    vector<int> parent;
    vector<int> rank;
};
int main()
{
    // int village_count, road_count = 0;
    // int cost_count;
    // // vector<vector<int>> table;
    // int table[village_count][village_count];
    // for (int i = 0; i < village_count; i++)
    // {
    //     for (int j = 0; j < village_count; j++)
    //     {
    //         table[i][j] = INT_MAX;
    //     }
    // }
    // for (int i = 0; i < road_count; i++)
    // {
    //     int village1, village2, cost;
    //     cin >> village1 >> village2 >> cost;
    //     // table[village1][village2] = cost;
    //     table[village1][village2] = cost;
    // }

    return 0;
}
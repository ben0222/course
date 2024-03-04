#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
// 边的数据结构
struct edge
{
    int village1;
    int village2;
    int cost;
};

// 比较器
bool compare(const edge &a, const edge &b)
{
    return a.cost < b.cost;
}

// 并查集类
class UnionFind
{
public:
    // 构造函数：rank数组全部初始化为0
    UnionFind(int n) : parent(n), rank(n, 0)
    {
        // parent数组：每个元素的parent初始化为自己
        for (int i = 0; i < n; i++)
            parent[i] = i;
    }
    // 查找元素x的parent
    int find(int x)
    {
        // 若parent不是本身，则继续查找这个parent的parent
        if (parent[x] != x)
        {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }
    // 关联x和y
    void unite(int x, int y)
    {
        int rootX = find(x);
        int rootY = find(y);
        if (rootX != rootY)
        {
            if (rank[rootX] < rank[rootY])
                parent[rootX] = rootY;
            else if (rank[rootX] > rank[rootY])
                parent[rootY] = rootX;
            else
            {
                parent[rootY] = rootX;
                rank[rootX]++;
            }
        }
    }
    // 判断x和y是否有关联
    bool connected(int x, int y)
    {
        return find(x) == find(y);
    }

private:
    vector<int> parent;
    vector<int> rank;
};

int kruskal(int village_count, vector<edge> &roads)
{
    sort(roads.begin(), roads.end(), compare); // 按照边的成本升序排序
    UnionFind uf(village_count + 1);
    int min_cost = 0;
    int road_count = 0;
    for (const auto &road : roads)
    {
        // 遍历路集合中每一条路，进行并查集判断，没有在一个连通集就将其连起来，并将其成本累加到总成本
        if (!uf.connected(road.village1, road.village2))
        {
            uf.unite(road.village1, road.village2);
            min_cost += road.cost;
            road_count++;
        }
    }

    if (road_count < village_count - 1)
        return -1; // 最小生成树理论，n条村至少要n-1条路

    return min_cost;
}
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
    int village_count, road_count = 0;
    vector<edge> roads;
    cin >> village_count >> road_count;

    for (int i = 0; i < road_count; i++)
    {
        int village1, village2, cost;
        cin >> village1 >> village2 >> cost;
        roads.push_back({village1, village2, cost});
    }

    int min_cost = kruskal(village_count, roads);
    cout << min_cost << endl;

    return 0;
}
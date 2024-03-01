#include <bits/stdc++.h>
using namespace std;

class DSU
{
    int *parent;
    int *rank;

public:
    DSU(int n)
    {
        parent = new int[n];
        rank = new int[n];

        for (int i = 0; i < n; i++)
        {
            parent[i] = -1;
            rank[i] = 1;
        }
    }

    int find(int i)
    {
        if (parent[i] == -1)
            return i;

        return parent[i] = find(parent[i]);
    }

    void unite(int x, int y)
    {
        int s1 = find(x);
        int s2 = find(y);

        if (s1 != s2)
        {
            if (rank[s1] < rank[s2])
            {
                parent[s1] = s2;
            }
            else if (rank[s1] > rank[s2])
            {
                parent[s2] = s1;
            }
            else
            {
                parent[s2] = s1;
                rank[s1] += 1;
            }
        }
    }
};

class Graph
{
    vector<vector<int>> adjacencyMatrix;
    int V;

public:
    Graph(int V)
    {
        this->V = V;
        adjacencyMatrix.resize(V, vector<int>(V, 0));
    }

    void addEdge(int x, int y, int w)
    {
        adjacencyMatrix[x][y] = w;
        adjacencyMatrix[y][x] = w;
    }

    void kruskals_mst()
    {
        vector<tuple<int, int, int>> edgelist;

        // Create a tuple for each edge (weight, x, y)
        for (int i = 0; i < V; ++i)
        {
            for (int j = 0; j < V; ++j)
            {
                if (adjacencyMatrix[i][j] != 0)
                {
                    edgelist.push_back({adjacencyMatrix[i][j], i, j});
                }
            }
        }

        // Sort all edges
        sort(edgelist.begin(), edgelist.end());

        DSU s(V);
        int ans = 0;
        cout << "Following are the edges in the "
             << "constructed MST"
             << endl;
        for (auto edge : edgelist)
        {
            int w = get<0>(edge);
            int x = get<1>(edge);
            int y = get<2>(edge);

            if (s.find(x) != s.find(y))
            {
                s.unite(x, y);
                ans += w;
                cout << x << " -- " << y << " == " << w << endl;
            }
        }
        cout << "Minimum Cost Spanning Tree: " << ans;
    }
};

int main()
{
    Graph g(4);
    g.addEdge(0, 1, 10);
    g.addEdge(1, 3, 15);
    g.addEdge(2, 3, 4);
    g.addEdge(2, 0, 6);
    g.addEdge(0, 3, 5);

    g.kruskals_mst();

    return 0;
}

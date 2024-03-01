#include <iostream>
#include <stack>
#include <vector>
#include <algorithm>

using namespace std;

class Graph
{
private:
    int V;
    vector<vector<int>> adjMatrix;
    // check if adjMatrix[v][i] != 0 to judge if v-j is merged.
    void topologicalSortUtil(int v, vector<bool> &visited, stack<int> &Stack);

public:
    Graph(int V);

    void addEdge(int v, int w);

    void topologicalSort();
};

Graph::Graph(int V)
{
    this->V = V;
    // Initialize the adjacency matrix with zeros
    adjMatrix.resize(V, vector<int>(V, 0));
}

void Graph::addEdge(int v, int w)
{
    // Assuming the graph is directed
    adjMatrix[v][w] = 1;
}

void Graph::topologicalSortUtil(int v, vector<bool> &visited, stack<int> &Stack)
{
    visited[v] = true;

    for (int i = 0; i < V; ++i)
    {
        if (!visited[i] && adjMatrix[v][i] != 0)
        {
            topologicalSortUtil(i, visited, Stack);
        }
    }

    Stack.push(v);
}

void Graph::topologicalSort()
{
    stack<int> Stack;
    vector<bool> visited(V, false);

    for (int i = 0; i < V; ++i)
    {
        if (!visited[i])
        {
            topologicalSortUtil(i, visited, Stack);
        }
    }

    while (!Stack.empty())
    {
        cout << Stack.top() << " ";
        Stack.pop();
    }
}

int main()
{
    Graph g(6);
    g.addEdge(5, 2);
    g.addEdge(5, 0);
    g.addEdge(4, 0);
    g.addEdge(4, 1);
    g.addEdge(2, 3);
    g.addEdge(3, 1);

    cout << "Following is a Topological Sort of the given graph\n";

    g.topologicalSort();

    return 0;
}

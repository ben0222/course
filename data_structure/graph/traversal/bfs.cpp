// C++ code to print BFS traversal from a given
// source vertex

#include <iostream>
#include <vector>
#include <list>
using namespace std;

// This class represents a directed graph using
// adjacency list representation
class Graph
{

	// No. of vertices
	int V;

	// Pointer to an array containing adjacency lists
	vector<list<int>> adj;

public:
	// Constructor
	Graph(int V);

	// Function to add an edge to graph
	void addEdge(int v, int w);

	// Prints BFS traversal from a given source s
	void BFS(int s);
};

Graph::Graph(int V)
{
	this->V = V;
	adj.resize(V);
}

void Graph::addEdge(int v, int w)
{
	// Add w to v’s list.
	adj[v].push_back(w);
}

// BFS begun from the current node s
void Graph::BFS(int v)
{
	// preperation
	//  Mark all the vertices as not visited
	vector<bool> visited;
	visited.resize(V, false);
	// create a queue for BFS
	list<int> queue;

	// Mark the current node as visited and enqueued it
	visited[v] = true;
	queue.push_back(v);

	while (!queue.empty())
	{
		// Dequeue a vertex from queue and print it
		v = queue.front();
		cout << v << " ";
		queue.pop_front();

		// Get all the adjacent vertices of the dequeued vertex s
		// If an adjacent has not been visited,
		// then mark it visited and enqueue it
		for (auto adjacent : adj[v])
		{
			if (visited[adjacent] == false)
			{
				visited[adjacent] = true;
				queue.push_back(adjacent);
			}
		}
	}
}

// Driver code
int main()
{
	// Create a graph given in the above diagram
	Graph g(4);
	g.addEdge(0, 1);
	g.addEdge(0, 2);
	g.addEdge(1, 2);
	g.addEdge(2, 0);
	g.addEdge(2, 3);
	g.addEdge(3, 3);

	cout << "Following is Breadth First Traversal "
		 << "(starting from vertex 2) \n";
	g.BFS(2);

	return 0;
}

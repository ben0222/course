// Use any below method to represent a Threaded Node

// Method 1: Using "struct" to make
// user-define data type
#include <cstddef>
struct node {
	int data;
	struct node* left;
	struct node* right;
	bool rightThread;
};

// Method 2: Using "class" to make
// user-define data type
class Node {
public:
	int data;
	Node* left;
	Node* right;
	bool rightThread;
	// Val is the key or the value that has to be added to
	// the data part
	Node(int val){
		data = val;
		// Left and right child for node will be initialized
		// to null
		left = NULL;
		right = NULL;
		// rightThread will be initialized to false
		rightThread = false;
	}
};

// This code is contributed by Susobhan Akhuli

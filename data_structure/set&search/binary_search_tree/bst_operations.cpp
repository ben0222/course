#include <iostream>
using namespace std;
struct BSTNode
{
	int data;
	BSTNode *left;
	BSTNode *right;
	BSTNode()
	{
		data = -1;
		left = NULL;
		right = NULL;
	}
	BSTNode(int data)
	{
		this->data = data;
		left = NULL;
		right = NULL;
	}
};
class BST
{
private:
	BSTNode *root;

public:
	BST()
	{
		this->root = new BSTNode(4);
		this->root->left = new BSTNode(3);
		this->root->left->left = new BSTNode(1);
		this->root->right = new BSTNode(7);
		this->root->right->right = new BSTNode(8);
	}
	// 以根为root的二叉搜索树中查找key，若找到则返回此结点，否则返回null
	BSTNode *search(int key, BSTNode *root)
	{
		if (root == NULL)
			return NULL;

		else if (key > root->data)
			return search(key, root->right);

		else if (key < root->data)
			return search(key, root->left);

		else
			return root;
	}
	// BSTNode *&root is a reference of a pointer, by using it the function could modify the pointer.
	void insert(BSTNode *&root, int value)
	{
		BSTNode *&p = root;
		while (p != NULL)
		{
			if (value < p->data)
			{
				p = p->left;
				continue;
			}
			else if (value > p->data)
			{
				p = p->right;
				continue;
			}
			else
				return;
		}
		p = new BSTNode(value);
	}
	bool insert(const int &value, BSTNode *&ptr)
	{
		if (ptr == nullptr)
		{
			ptr = new BSTNode(value);
			cout << "插入成功" << endl;
			return true;
		}
		else if (value < ptr->data)
			return insert(value, ptr->left);
		else if (value > ptr->data)
			return insert(value, ptr->right);
		else
			return false;
	}
	BSTNode *&getRoot()
	{
		return this->root;
	}
};
int main()
{
	return 0;
}
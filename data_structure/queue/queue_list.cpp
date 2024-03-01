#include <iostream>
using namespace std;
struct ListNode
{
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

class ListQueue
{
private:
    ListNode *front, *rear;
    int queSize;

public:
    ListQueue()
    {
        front = nullptr;
        rear = nullptr;
        queSize = 0;
    }

    int size()
    {
        return queSize;
    }

    bool empty()
    {
        return queSize == 0;
    }

    int peek()
    {
        if (size() == 0)
        {
            throw out_of_range("the queue is empty.");
        }
        else
            return front->val;
    }

    void push(int num)
    {
        ListNode *node = new ListNode(num);

        // if queue is empty, let *front and *rear point to this node
        if (front == nullptr)
        {
            front = node;
            rear = node;
        }
        // if not, let the node be the new *rear
        else
        {
            rear->next = node;
            rear = node;
        }
        queSize++;
    }

    int pop()
    {
        int num = peek();
        ListNode *temp = front;
        // update front
        front = front->next;
        delete temp;
        queSize--;

        if (empty())
            rear = nullptr;

        return num;
    }
};
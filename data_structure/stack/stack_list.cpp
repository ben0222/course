#include <iostream>
using namespace std;
struct ListNode
{
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};
class ListStack
{
private:
    int stkSize;
    ListNode *stackTop;

public:
    ListStack() : stkSize(0), stackTop(nullptr) {}

    void push(int num)
    {
        ListNode *newTop = new ListNode(num);
        newTop->next = stackTop;
        stackTop = newTop;
        stkSize++;
    }

    int pop()
    {
        int num = top();
        ListNode *temp = stackTop;
        stackTop = stackTop->next;
        stkSize--;
        delete temp;
        return num;
    }

    int top()
    {
        if (size() == 0)
        {
            throw out_of_range("the stack is empty.");
        }
        return stackTop->val;
    }
    int size()
    {
        return stkSize;
    }
};
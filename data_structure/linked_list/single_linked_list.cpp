#include <iostream>
using namespace std;

struct LinkNode
{
    int data;
    LinkNode *next;
    LinkNode(int d = 0)
    {
        data = d;
        next = nullptr;
    }
};

class List
{
private:
    LinkNode *head;

public:
    List()
    {
        head = new LinkNode();
    }
    List(int d)
    {
        head = new LinkNode(d);
    }
    LinkNode *search(int x) // search the node with data x, return its pointer
    {
        LinkNode *current = head->next;
        while (current != nullptr)
        {
            if (current->data == x)
            {
                break;
            }
            else
                current = current->next;
        }

                return current;
    }

    LinkNode *locate(int no) // return no.x node's pointer
    {
        LinkNode *current = head->next;
        int count = 0;
        while (count < no && current != nullptr)
        {
            current = current->next;
            count++;
        }
        return current;
    }

    LinkNode *insert(int no, LinkNode *p)
    {
        if (no < 0)
            return nullptr;
        LinkNode *current = locate(no - 1);
        if (current != nullptr)
        {
            LinkNode *temp = current->next;
            current->next = p;
            p->next = temp;
        }
        cout << "insert was successful" << endl;
        return current;
    }
    bool remove(int no) // remove no.x node from the list
    {
        LinkNode *current = locate(no - 1);
        if (current == nullptr)
            return 0;

        LinkNode *del = current->next;
        current->next = del->next;
        delete del;
        cout << "remove was successful" << endl;
        return 1;
    }
};

int main()
{
    // 创建链表
    List myList;

    // 插入节点
    LinkNode *node1 = new LinkNode(10);
    myList.insert(0, node1);

    LinkNode *node2 = new LinkNode(20);
    myList.insert(1, node2);

    LinkNode *node3 = new LinkNode(30);
    myList.insert(2, node3);

    myList.search(10);
    myList.remove(2);
    myList.search(10);

    return 0;
}
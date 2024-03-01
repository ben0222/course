#include <iostream>
using namespace std;

struct ListNode
{
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

// 逆置链表函数
ListNode *reverseList(ListNode *head)
{
    ListNode *prev = nullptr;
    ListNode *current = head;
    ListNode *next = nullptr;

    while (current != nullptr)
    {
        next = current->next; // 保存下一个节点的指针
        current->next = prev; // 将当前节点的next指向前一个节点
        prev = current;       // 更新前一个节点指针
        current = next;       // 更新当前节点指针
    }

    return prev; // prev现在是逆置后的头节点
}

// 打印链表
void printList(ListNode *head)
{
    ListNode *current = head;
    while (current != nullptr)
    {
        cout << current->val << " ";
        current = current->next;
    }
    cout << endl;
}

int main()
{
    // 创建链表
    ListNode *head = new ListNode(1);
    head->next = new ListNode(2);
    head->next->next = new ListNode(3);
    head->next->next->next = new ListNode(4);
    head->next->next->next->next = new ListNode(5);

    cout << "Original List: ";
    printList(head);

    // 逆置链表
    head = reverseList(head);

    cout << "Reversed List: ";
    printList(head);

    return 0;
}

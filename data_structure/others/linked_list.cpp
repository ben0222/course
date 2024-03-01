#include <iostream>
using namespace std;
 struct ListNode
 {
    int val;
    ListNode *next;
    ListNode(int x):val(x),next(nullptr){}
 };
 struct ListNode2
 {
      int val;
      ListNode *next;
      ListNode *prev;
      Listnode(int x):val(x),next(nullptr),prev(nullptr){}
 }
 //在结点n0之后插入节点P
 void insert(ListNode *n0,ListNode *P)
 {
    ListNode *n1 = n0->next;
    P->next = n1;
    n0->next = P;
 }
 //删除n0后的第一个节点
 void remove(ListNode *n0)
 {
    if(n0->next == nullptr) return;//如果n0下一个为空，即n0是尾结点，没有下一个
    //n0->P->n1,利用P指针找到n0后的第二个节点n1，然后连接n0和n1
    ListNode *P = n0->next;
    ListNode *n1 = P->next;
    n0->next = n1;
    delete P;
 }
 //访问链表中索引为 index 的节点
 ListNode *access(ListNode *head, int index)
 {
    for(int i = 0; i< index; i++)
    {
        if(head == nullptr) return nullptr;
        head = head->next; 
    }
    return head;
 }
 int main()
 {
    /* 初始化链表 1 -> 3 -> 2 -> 5 -> 4 */
    // 初始化各个节点
    ListNode* n0 = new ListNode(1);
    ListNode* n1 = new ListNode(3);
    ListNode* n2 = new ListNode(2);
    ListNode* n3 = new ListNode(5);
    ListNode* n4 = new ListNode(4);
    // 构建引用指向
    n0->next = n1;
    n1->next = n2;
    n2->next = n3;
    n3->next = n4;
 }
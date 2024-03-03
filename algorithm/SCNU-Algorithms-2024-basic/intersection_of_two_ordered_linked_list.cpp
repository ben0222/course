#include <iostream>
#include <vector>
using namespace std;

struct ListNode
{
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(NULL) {}
};

// 双指针法：
// 初始化两个指针分别指向S1和S2的头部。
// 比较指针所指的元素大小：
//  ·如果两个元素相等，则将该元素添加到S3中，然后将两个指针都向后移动一位。
//  ·如果其中一个元素小于另一个，则将小的元素所在的链表的指针向后移动一位。
// 重复步骤2直到有一个链表遍历完毕。
ListNode *intersection(ListNode *head1, ListNode *head2)

{
    ListNode *head3 = new ListNode(0);
    ListNode *tail3 = head3;

    while (head1 && head2)
    {
        if (head1->val == head2->val)
        {
            tail3->next = new ListNode(head1->val);
            tail3 = tail3->next;
            head1 = head1->next;
            head2 = head2->next;
        }
        else if (head1->val < head2->val)
            head1 = head1->next;
        else
            head2 = head2->next;
    }

    ListNode *result = head3->next;
    delete head3;
    return result;
}
// 从输入得到的vector构建列表
ListNode *constructor(vector<int> v)
{
    ListNode *head = nullptr;
    // ListNode *current;
    for (int i = v.size() - 1; i >= 0; i--)
    {
        ListNode *node = new ListNode(v[i]);
        node->next = head;
        head = node;
    }
    return head;
}
int main()
{
    vector<int> s1, s2;

    int num;
    while (cin >> num && num != -1)
    {
        s1.push_back(num);
    }
    while (cin >> num && num != -1)
    {
        s2.push_back(num);
    }

    ListNode *s1_linked_list = constructor(s1);
    ListNode *s2_linked_list = constructor(s2);
    ListNode *s3 = intersection(s1_linked_list, s2_linked_list);

    if (!s3)
    {
        cout << "NULL" << endl;
    }
    else
    {
        while (s3)
        {
            cout << s3->val;
            if (s3->next)
            {
                cout << " ";
            }
            s3 = s3->next;
        }
        cout << endl;
        // 释放链表内存
        while (s1_linked_list)
        {
            ListNode *temp = s1_linked_list;
            s1_linked_list = s1_linked_list->next;
            delete temp;
        }

        while (s2_linked_list)
        {
            ListNode *temp = s2_linked_list;
            s2_linked_list = s2_linked_list->next;
            delete temp;
        }
    }
    return 0;
}

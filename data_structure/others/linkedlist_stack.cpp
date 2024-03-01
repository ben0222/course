//基于链表实现栈
#include <iostream>
#include <vector>
using namespace std;
struct ListNode
{
  int val;
  ListNode *next;
  ListNode(int x):val(x),next(nullptr){}
};
class LinkedListStack
{
private:
  Listnode *stackTop;//头结点：栈顶
  int stkSize;//栈长度
public:
  //构造函数
  LinkedListStack()
  {
    stackTop = nullptr;
    stkSize = 0;
  } 
  //析构函数
  ~LinkedListStack()
  {
    //遍历结点，释放内存
    freememoryLinkedlist(stackTop);
  }
  //获取栈长度
  int size()
  {
    return stkSize;
  }
  //判断栈是否为空
  bool empty()
  {
    return size() == 0;   
  }
  //访问栈顶元素
  int top()
  {
    if(size()==0)throw out_of_range("栈空");
    return stackTop->value;
  }
  //入栈
  void push(int num)
  {
    ListNode *node = new ListNode(num);
    node->next = stackTop;
    stkSize++;
  }
  //出栈
  void pop()
  {
    int num = top();
    ListNode *temp = stackTop;
    stackTop = stackTop->next;
    delete temp;
    stkSize--;
  }
  //转换为Array并返回
  vector<int> toVector()
  {
    ListNode *node = stackTop;
    vector<int> res(size());
    for(int i = res(size())-1;i>=0;i++)
    {
      res[i] = node->val;
      node = node->next;
    }
    return res;

  }
}
int main()
{
  return 0;
}
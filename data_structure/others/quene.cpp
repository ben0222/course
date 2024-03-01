#include <iostream>
#include <algorithm>
#include <queue>
using namespace std;
int main()
{
  queue<int> q1;
  for(int i = 0; i < 5; i++)
  {
    q1.push(i);
  }
  int front = q1.front();
  cout << front << endl;
  q1.pop();
  cout << front << endl;
  int size = q1.size();
  bool empty = q1.empty();
  cout << "size:" << size << endl;
  cout << "empty? :" << empty << endl;
  return 0;
}
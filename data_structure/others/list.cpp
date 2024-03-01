#include <iostream>
#include <vector>
using namespace std;
int main()
{
  vector<int> list = {1 , 3 , 2 , 5 , 4};
  int num = list[1];
  list[1] = 0;
  list.clear();
  list.push_back(1);
  list.push_back(3);
  list.push_back(2);
  list.push_back(5);
  list.push_back(4);
  list.insert(list.begin()+ 3 , 6);
  list.erase(list.begin() + 3);
  int count = 0;
  for(int n : list)
  {
    count++;
  }
  return 0;
}
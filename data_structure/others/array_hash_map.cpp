#include <iostream>
#include<bits/stdc++.h>
using namespace std;
/*键值对*/
struct Pair
{
  public:
  int key;
  string val;
  Pair(int key, string val)
  {
    this->key = key;
    this->val = val;
  }
};
/*数组实现哈希表*/
class ArrayHashMap
{
  private:
  vector<Pair *> buckets;
  public:
  ArrayHashMap()
  {
    //初始化100桶
    buckets = vector<Pair *>(100);
  }
  ~ArrayHashMap(){//释放内存
  for(const auto&bucket:buckets)
  {
    delete bucket;
  }
  buckets.clear();
  }
  int hashFunc(int key)
  {
    int index = hashFunc(key);
    Pair *pair = buckets[index];
    if(pair == nullptr)
      return 0;
    return pair->val;

  }
};
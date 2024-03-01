#include <iostream>
//#include <hash_map>
#include <unordered_map>
//#include <map>
using namespace std;
int main()
{
  unordered_map<int, string> map;
  map[10001] = "alex";
  map[10002] = "ben";
  cout << map[10001] << endl;
  for(auto kv:map)
  {
    cout << kv.first << "->" << kv.second << endl;
  }
  return 0;
}
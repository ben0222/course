// 1.算术表达式

#include <iostream>

using namespace std;

// 单词种类的枚举
enum TokenID
{
  NUMBER,
  ADD,
  SUB,
  MUL,
  DIV,
  LBRACKET,
  RBRACKET,
  ENDINPUT
};

// 单词结构
struct TokenStru
{
  TokenID ID;
  int val;
  char op;
};

TokenStru token;
char buffer[255];
int pos = 0;

// 词法分析函数：算术表达式的词法分析，

void GetToken()
{
  int s;

  while (buffer[pos] == 32 || buffer[pos] == 9) // spacebar or tab
    pos++;

  if ((buffer[pos] >= '0') && (buffer[pos] <= '9')) // digit
  {
    s = 0;
    while (buffer[pos] >= '0' && buffer[pos] <= '9')
    {
      s = s * 10 + buffer[pos] - '0';
      pos++;
    }
    token.ID = NUMBER;
    token.val = s;
  }
  // 假设要处理123这个字符串，

  // 当我们处理第一个字符 '1' 时，我们得到的数字是 1，因为 '1' 的 ASCII 值减去 '0' 的 ASCII 值为 1。所以当前数字是 1。

  // 当我们处理第二个字符 '2' 时，我们需要将之前已识别的数字乘以 10，然后再加上当前字符所代表的数字。即 1 * 10 + 2 = 12。现在我们得到的数字是 12。

  // 接着处理第三个字符 '3'，同样的操作，将之前的数字乘以 10，然后加上当前字符所代表的数字。即 12 * 10 + 3 = 123。现在我们得到的数字是 123。
  else if (buffer[pos] != '\0')
  {
    switch (buffer[pos])
    {
    case '+':
      token.ID = ADD;
      token.op = buffer[pos];
      break; // +
    case '-':
      token.ID = SUB;
      token.op = buffer[pos];
      break; // -
    case '*':
      token.ID = MUL;
      token.op = buffer[pos];
      break; // *
    case '/':
      token.ID = DIV;
      token.op = buffer[pos];
      break; // /
    case '(':
      token.ID = LBRACKET;
      token.op = buffer[pos];
      break;
    case ')':
      token.ID = RBRACKET;
      token.op = buffer[pos];
      break;
    default:
      cout << " Error Input at: " << pos + 1;
      exit(1);
    }
    pos++;
  }
  else
    token.ID = ENDINPUT;
}

void main()
{
  cin.getline(buffer, 255); // 读入一行的算术表达式
  pos = 0;

  GetToken(); // 获取第一单词
  cout << "The result is : " << endl;
  while (token.ID != ENDINPUT)
  {
    if (token.ID == 0)
      cout << token.ID << " " << token.val << endl;
    else
      cout << token.ID << " " << token.op << endl;
    GetToken(); // 获取下一个单词
  }
  system("pause");
}
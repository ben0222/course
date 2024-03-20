// 1.算术表达式
// 相比expLexl.cpp:可以识别算术表达式中的标识符（由字母数字下划线组成）
#include <iostream>

using namespace std;

// 单词种类的枚举
enum TokenID
{
  NUMBER,
  ID,
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
  char word[20]; // 用于存储识别出的标识符
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
  // 识别标识符
  else if (isalpha(buffer[pos])) // 如果是字母，则进入标识符判断
  {
    int len = 0;
    while (isalpha(buffer[pos]) || isdigit(buffer[pos]) || buffer[pos] == '_') // 如果连续的都是字母或数字或下划线
    {
      token.word[len] = buffer[pos]; // 在存储数组开始逐个字符存放
      len++;
      pos++;
    }
    token.word[len] = '\0';
    token.ID = ID;
  }
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
    if (token.ID == NUMBER)
      cout << token.ID << " " << token.val << endl;
    else if (token.ID == ID)
      cout << token.ID << " " << token.word << endl;
    else
      cout << token.ID << " " << token.op << endl;
    GetToken(); // 获取下一个单词
  }
  system("pause");
}
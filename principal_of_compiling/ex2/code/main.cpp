#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <set>
#include <map>
#include <unordered_map>
#include <stack>
using namespace std;
/*----------数据结构----------*/
const string EPSILON = "~";
// 结点数量统计
int node_count;
// 全局字符统计
set<char> NFAChar;
set<char> DFAChar;
struct NFAedge // NFA边
{
    string type;
    NFAnode *next;
};
struct NFAnode // NFA结点
{
    int id;       // 结点编号
    bool isInit;  // 是否为初态
    bool isFinal; // 是否为末态
    vector<NFAedge> edges;
    NFAnode()
    {
        id = node_count++;
        isInit = false;
        isFinal = false;
    }
};

struct NFA // NFA图
{
    NFAnode *start;
    NFAnode *end;
    NFA() {}
    NFA(NFAnode *s, NFAnode *e)
    {
        start = s;
        end = e;
    }
};

struct statusTableNode
{
    string flag;           // 标记初态还是终态
    int id;                // 唯一id值
    map<char, set<int>> m; // 对应字符能到达的状态
    statusTableNode()
    {
        flag = ""; // 默认为空
    }
};

// 全局存储状态转换表
unordered_map<int, statusTableNode> statusTable;
// statusTable插入顺序记录，方便后续输出
vector<int> insertionOrder;
set<int> startNFAstatus;
set<int> endNFAstatus;

/*----------预处理----------*/
string multiLine(const string &regex)
{
    vector<string> lines;
    string line;
    istringstream iss(regex);

    while (getline(iss, line))
    {
        if (!line.empty())
        {
            lines.push_back(line);
        }
    }

    string output;

    for (size_t i = 0; i < lines.size(); ++i)
    {
        output += "(" + lines[i] + ")";
        if (i < lines.size() - 1)
        {
            output += "|";
        }
    }

    return output;
}
// 判断是否为合法字符
bool isLegal(char c)
{
    if (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z')
        return true;
    return false;
}

/*----------正则表达式转NFA----------*/
// 处理正则表达式
string regexHandling(string regex)
{
    cout << "The regex you enter" << regex;
    string result;
    bool inside_brackets = false;
    string cur_str;

    for (char c : regex)
    {
        if (c == '[')
        {
            inside_brackets = true;
            cur_str.push_back('(');
        }
        else if (c == ']')
        {
            inside_brackets = false;
            cur_str.push_back(')');
            result += cur_str;
            cur_str.clear();
        }
        else if (inside_brackets)
        {
            if (cur_str.length() > 1)
            {
                cur_str.push_back('|');
            }
            cur_str.push_back(c);
        }
        else
        {
            result.push_back(c);
        }
    }

    regex = result;

    for (int i = 0; i < regex.size(); i++)
    {
        if (regex[i] == '+')
        {
            int kcount = 0;
            int j = i;
            do
            {
                j--;
                if (regex[j] == ')')
                {
                    kcount++;
                }
                else if (regex[j] == '(')
                {
                    kcount--;
                }
            } while (kcount != 0);
            string s1 = regex.substr(0, j);
            string s2 = regex.substr(j, i - j);
            string s3 = regex.substr(i + 1, regex.size() - i);
        }
    }

    for (int i = 0; i < regex.size() - 1; i++)
    {
        if (isLegal(regex[i]) && isLegal(regex[i + 1]) || isLegal(regex[i]) && regex[i + 1] == '(' || regex[i] == ')' && isLegal(regex[i + 1]) || regex[i] == ')' && regex[i + 1] == '(' || regex[i] == '*' && regex[i + 1] != ')' && regex[i + 1] != '|' && regex[i + 1] != '?' || regex[i] == '?' && regex[i + 1] != ')' || regex[i] == '+' && regex[i + 1] != ')')
        {
            string str1 = regex.substr(0, i + 1);
            string str2 = regex.substr(i + 1, (regex.size() - i));
            str1 += "."; // 连接
            regex = str1 + str2;
        }
    }

    return regex;
}

int priority(char op)
{
    if (op == '|')
    {
        return 1; // 选择运算 | 的优先级
    }
    else if (op == '.') // 连接
    {
        return 2;
    }
    else if (op == '*' || op == '?')
    {
        return 3; // 闭包运算 * 和可选运算 ? 的优先级
    }
    else
    {
        return 0; // 默认情况设为0
    }
}
// 点a--c-->点b
NFA NFA_basic(char c)
{
    /*---新的初态和末态的定义---*/
    NFAnode *start = new NFAnode();
    NFAnode *end = new NFAnode();

    // 确定初态和末态
    start->isInit = true;
    end->isFinal = true;

    /*---边的定义---*/
    NFAedge edge;
    edge.type = c;
    edge.next = end;
    start->edges.push_back(edge);

    /*---图的定义---*/
    NFA nfa(start, end);

    NFAChar.insert(c);
    DFAChar.insert(c);

    return nfa;
}
// 连接
NFA NFA_connect(NFA nfa_a, NFA nfa_b)
{
    nfa_a.end->isFinal = false;
    nfa_b.start->isInit = false;

    NFAedge edge1;
    edge1.type = EPSILON;
    edge1.next = nfa_b.start;
    nfa_a.end->edges.push_back(edge1);

    NFA nfa;
    nfa.start = nfa_a.start;
    nfa.end = nfa_b.end;

    return nfa;
}
// 选择运算：|
NFA NFA_or(NFA nfa_a, NFA nfa_b)
{
    /*---新的初态和末态的定义---*/
    NFAnode *start = new NFAnode();
    NFAnode *end = new NFAnode();

    // 确定初态和末态
    start->isInit = true;
    end->isFinal = true;

    // 把新的初态与nfa1和nfa2的初态连接起来
    NFAedge edge1;
    edge1.type = EPSILON;
    edge1.next = nfa_a.start;
    start->edges.push_back(edge1);
    nfa_a.start->isInit = false;

    NFAedge edge2;
    edge2.type = EPSILON;
    edge2.next = nfa_b.start;
    start->edges.push_back(edge2);
    nfa_b.start->isInit = false;

    // 把nfa1和nfa2的终止状态与新的终止状态连接起来
    NFAedge edge3;
    edge3.type = EPSILON;
    edge3.next = end;
    nfa_a.end->edges.push_back(edge3);
    nfa_a.end->isFinal = false;

    NFAedge edge4;
    edge4.type = EPSILON;
    edge4.next = end;
    nfa_b.end->edges.push_back(edge4);
    nfa_b.end->isFinal = false;

    NFA nfa(start, end); // 就是上面新的初态和末态
    return nfa;
}
// 可选运算：？
NFA NFA_optional(NFA nfa_a)
{
    /*---新的初态和末态的定义---*/
    NFAnode *start = new NFAnode();
    NFAnode *end = new NFAnode();

    // 确定初态和末态
    start->isInit = true;
    end->isFinal = true;

    // 新初态和nfa_a的初态进行EPOSILON连接
    NFAedge edge1;
    edge1.type = EPSILON;
    edge1.next = nfa_a.start;
    start->edges.push_back(edge1);
    nfa_a.start->isInit = false;

    // nfa_a的终态和新末态进行EPOSILON连接
    NFAedge edge2;
    edge2.type = EPSILON;
    edge2.next = end;
    nfa_a.end->edges.push_back(edge2);
    nfa_a.end->isFinal = false;

    // 可选运算的实现：新初态和新末态进行EPOSILON连接
    NFAedge edge3;
    edge3.type = EPSILON;
    edge3.next = end;
    start->edges.push_back(edge3);
}

// 闭包运算零次或多次：*
NFA NFA_kleene_star(NFA nfa_a)
{
    /*---新的初态和末态的定义---*/
    NFAnode *start = new NFAnode();
    NFAnode *end = new NFAnode();

    // 确定初态和末态
    start->isInit = true;
    end->isFinal = true;

    // 新初态和nfa_a的初态进行EPOSILON连接
    NFAedge edge1;
    edge1.type = EPSILON;
    edge1.next = nfa_a.start;
    start->edges.push_back(edge1);
    nfa_a.start->isInit = false;

    // nfa_a的终态和新末态进行EPOSILON连接
    NFAedge edge2;
    edge2.type = EPSILON;
    edge2.next = end;
    nfa_a.end->edges.push_back(edge2);
    nfa_a.end->isFinal = false;

    // 闭包运算的实现：
    //  新初态和新末态进行EPOSILON连接
    NFAedge edge3;
    edge3.type = EPSILON;
    edge3.next = end;
    start->edges.push_back(edge3);

    NFAedge edge4;
    edge4.type = EPSILON;
    edge4.next = end;
    nfa_a.end->edges.push_back(edge4);

    NFA nfa(start, end);
    return nfa;
}

// 对NFA图进行DFS，形成状态转换表
void createNFAStatusTable(NFA &nfa)
{
    stack<NFAnode *> NFAstack;
    set<NFAnode *> visited_nodes;

    // 初态
    NFAnode *startNode = nfa.start;
    statusTableNode startStatusNode;
    startStatusNode.flag = '-'; // -表示初态
    startStatusNode.id = startNode->id;
    statusTable[startNode->id] = startStatusNode;
    insertionOrder.push_back(startNode->id);
    startNFAstatus.insert(startNode->id);

    NFAstack.push(startNode);

    while (!NFAstack.empty())
    {
        NFAnode *currentNode = NFAstack.top();
        NFAstack.pop();
        visited_nodes.insert(currentNode);

        for (NFAedge edge : currentNode->edges)
        {
            char transitionChar = edge.c;
            NFAnode *nextNode = edge.next;

            // 记录状态转换信息
            statusTable[currentNode->id].m[transitionChar].insert(nextNode->id);

            // 如果下一个状态未被访问，将其加入堆栈
            if (visited_nodes.find(nextNode) == visited_nodes.end())
            {
                NFAstack.push(nextNode);

                // 记录状态信息
                statusTableNode nextStatus;
                nextStatus.id = nextNode->id;
                if (nextNode->isInit)
                {
                    nextStatus.flag += '-'; // -表示初态
                    startNFAstatus.insert(nextStatus.id);
                }
                else if (nextNode->isFinal)
                {
                    nextStatus.flag += '+'; // +表示终态
                    endNFAstatus.insert(nextStatus.id);
                }
                statusTable[nextNode->id] = nextStatus;
                // 记录插入顺序（排除终态）
                if (!nextNode->isFinal)
                {
                    insertionOrder.push_back(nextNode->id);
                }
            }
        }
    }

    // 顺序表才插入终态
    nfaNode *endNode = nfa.end;
    insertionOrder.push_back(endNode->id);
}

int main()
{
    return 0;
}
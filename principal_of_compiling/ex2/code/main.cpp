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

int prec(char op)
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
    NFAnode *startNode = nfa.start;//初始化一个NFA图结点，用所传入的NFA图的start结点
    statusTableNode startStatusNode;//初始化一个状态表结点，为初态结点
    //设置该结点属性
    startStatusNode.flag = '-';//标记：-为初态 
    startStatusNode.id = startNode->id;//编号：为所传入NFA图start结点的编号
    statusTable[startNode->id] = startStatusNode;//建立编号和表结点的映射
    insertionOrder.push_back(startNode->id);//将该编号push入记录插入顺序的vector，来记录插入顺序
    startNFAstatus.insert(startNode->id);//由于这是初态结点，则需要insert入记录初态的set

    NFAstack.push(startNode);//将该NFA图结点push入栈内

    while (!NFAstack.empty())
    {
        NFAnode *currentNode = NFAstack.top();
        NFAstack.pop();
        visited_nodes.insert(currentNode);

        for (NFAedge edge : currentNode->edges)
        {
            char transition_type = edge.type;
            NFAnode *nextNode = edge.next;

            // 记录状态转换信息
            statusTable[currentNode->id].m[transition_type].insert(nextNode->id);

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
    NFAnode *endNode = nfa.end;
    insertionOrder.push_back(endNode->id);
}

NFA regexToNFA(string regex)
{
    // 双栈法，创建两个栈opStack（运算符栈）,nfaStack（nfa图栈）
    stack<char> opStack;
    stack<NFA> NFAStack;

    // 对表达式进行类似于逆波兰表达式处理
    // 运算符：| .（） ？ +  *
    for (char c : regex)
    {
        switch (c)
        {
        case ' ': // 空格跳过
            break;
        case '(':
            opStack.push(c);
            break;
        case ')':
            while (!opStack.empty() && opStack.top() != '(')
            {
                // 处理栈顶运算符，构建NFA图，并将结果入栈
                char op = opStack.top();
                opStack.pop();

                if (op == '|') {
                    // 处理并构建"|"运算符
                    NFA nfa2 = NFAStack.top();
                    NFAStack.pop();
                    NFA nfa1 = NFAStack.top();
                    NFAStack.pop();

                    // 创建新的NFA，表示nfa1 | nfa2
                    NFA nfa_result = NFA_or(nfa1, nfa2);
                    nfaStack.push(nfa_result);
                }
                else if (op == '.') {
                    // 处理并构建"."运算符
                    NFA nfa2 = NFAStack.top();
                    NFAStack.pop();
                    NFA nfa1 = NFAStack.top();
                    NFAStack.pop();

                    // 创建新的NFA，表示nfa1 . nfa2
                    NFA nfa_result = NFA_connect(nfa1, nfa2);
                    NFAStack.push(nfa_result);
                }
            }
            if (opStack.empty())
            {
                cout << "Brackets error!";
                exit(-1);
            }
            else
            {
                opStack.pop(); // 弹出(
            }
            break;
            //下面两个运算符的处理代码与上面while循环内一致！！！！！！！！！，需要修改
        case '|':
        case '.':
            // 处理运算符 | 和 .
            while (!opStack.empty() && (opStack.top() == '|' || opStack.top() == '.') &&
                prec(opStack.top()) >= prec(c))
            {
                char op = opStack.top();
                opStack.pop();

                // 处理栈顶运算符，构建NFA图，并将结果入栈
                if (op == '|') {
                    // 处理并构建"|"运算符
                    NFA nfa2 = nfaStack.top();
                    nfaStack.pop();
                    NFA nfa1 = nfaStack.top();
                    nfaStack.pop();

                    // 创建新的NFA，表示nfa1 | nfa2
                    NFA resultNFA = CreateUnionNFA(nfa1, nfa2);
                    nfaStack.push(resultNFA);
                }
                else if (op == '.') {
                    // 处理并构建"."运算符
                    NFA nfa2 = nfaStack.top();
                    nfaStack.pop();
                    NFA nfa1 = nfaStack.top();
                    nfaStack.pop();

                    // 创建新的 NFA，表示 nfa1 . nfa2
                    NFA resultNFA = CreateConcatenationNFA(nfa1, nfa2);
                    nfaStack.push(resultNFA);
                }
            }
            opStack.push(c);
            break;
        case '?':
        case '*':
            // 处理闭包运算符 ? + *
            // 从NFAStack弹出NFA，应用相应的闭包操作，并将结果入栈
            if (!NFAStack.empty()) {
                NFA nfa = NFAStack.top();
                NFAStack.pop();
                if (c == '?') {
                    // 处理 ?
                    NFA nfa_result = NFA_optional(nfa);
                    nfaStack.push(nfa_result);
                }
                else if (c == '*') {
                    // 处理 *
                    NFA nfa_result = NFA_kleene_star(nfa);
                    nfaStack.push(nfa_result);
                }
            }
            else {
                cout << "Kleene closure error!";
                exit(-1);
            }
            break;
        default:
            // 处理字母字符
            NFA nfa = NFA_basic(c); // 创建基本的字符NFA
            NFAStack.push(nfa);
            break;
        }

    }

    // 处理栈中剩余的运算符
    while (!opStack.empty())
    {
        char op = opStack.top();
        opStack.pop();

        if (op == '|' || op == '.')
        {
            // 处理并构建运算符 | 和 .
            if (nfaStack.size() < 2)
            {
                qDebug() << "正则表达式语法错误：不足以处理运算符 " << op << "！";
                exit(-1);
            }

            NFA nfa2 = nfaStack.top();
            nfaStack.pop();
            NFA nfa1 = nfaStack.top();
            nfaStack.pop();

            if (op == '|')
            {
                // 创建新的 NFA，表示 nfa1 | nfa2
                NFA resultNFA = CreateUnionNFA(nfa1, nfa2);
                nfaStack.push(resultNFA);
            }
            else if (op == '.')
            {
                // 创建新的 NFA，表示 nfa1 . nfa2
                NFA resultNFA = CreateConcatenationNFA(nfa1, nfa2);
                nfaStack.push(resultNFA);
            }
        }
        else
        {
            qDebug() << "正则表达式语法错误：未知的运算符 " << op << "！";
            exit(-1);
        }
    }

    // 最终的NFA图在nfaStack的顶部
    NFA result = nfaStack.top();
    qDebug() << "NFA图构建完毕" << endl;

    createNFAStatusTable(result);
    qDebug() << "状态转换表构建完毕" << endl;

    return result;
}

int main()
{
    return 0;
}
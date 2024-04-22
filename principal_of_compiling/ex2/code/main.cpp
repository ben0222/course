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
struct NFAnode;
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
    string flag;                           // 标记初态还是终态
    int id;                                // 唯一id值
    map<string, set<int>> state_reachable; // 对应字符能到达的状态
    statusTableNode()
    {
        flag = ""; // 默认为空
    }
};

// 全局存储状态转换表
unordered_map<int, statusTableNode> statusTable;
// statusTable插入顺序记录，方便后续输出
vector<int> insertionOrder;
set<int> initNFAstatus;
set<int> finalNFAstatus;

// dfa节点
struct DFAnode
{
    string flag;                       // 是否包含终态（+）或初态（-）
    set<int> nfaStates;                // 该DFA状态包含的NFA状态的集合
    map<string, set<int>> transitions; // 字符到下一状态的映射
    DFAnode()
    {
        flag = "";
    }
};
// dfa状态去重集
set<set<int>> dfaStatusSet;

// dfa最终结果
vector<DFAnode> dfaTable;

// 下面用于DFA最小化
//  dfa终态集合
set<int> dfaFinalStatusSet;
// dfa非终态集合
set<int> dfaNotFinalStatusSet;
// set对应序号MAP
map<set<int>, int> dfaToNumberMap;
int startStaus;
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
NFA NFA_alternation(NFA nfa_a, NFA nfa_b)
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

    // 把nfa_a和nfa_b的终止状态与新的终止状态连接起来
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
// 正闭包运算：+
NFA NFA_kleene_plus(NFA nfa_a)
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

    // 正闭包运算的实现：对比闭包运算，少了一条从新初态到新末态的epsilon连接
    //  闭包运算的实现：
    //   新初态和新末态进行EPOSILON连接
    //  NFAedge edge3;
    //  edge3.type = EPSILON;
    //  edge3.next = end;
    //  start->edges.push_back(edge3);

    NFAedge edge3;
    edge3.type = EPSILON;
    edge3.next = end;
    nfa_a.end->edges.push_back(edge3);

    NFA nfa(start, end);
    return nfa;
}

// 对NFA图进行DFS，形成状态转换表
void createNFAStatusTable(NFA &nfa)
{
    stack<NFAnode *> NFAStack;
    set<NFAnode *> visited_nodes;

    // 初态
    NFAnode *startNode = nfa.start;  // 初始化一个NFA图结点，用所传入的NFA图的start结点
    statusTableNode startStatusNode; // 初始化一个状态表结点，为初态结点
    // 设置该结点属性
    startStatusNode.flag = '-';                   // 标记：-为初态
    startStatusNode.id = startNode->id;           // 编号：为所传入NFA图start结点的编号
    statusTable[startNode->id] = startStatusNode; // 建立编号和表结点的映射
    insertionOrder.push_back(startNode->id);      // 将该编号push入记录插入顺序的vector，来记录插入顺序
    initNFAstatus.insert(startNode->id);          // 由于这是初态结点，则需要insert入记录初态的set

    NFAStack.push(startNode); // 将该NFA图结点push入栈内

    while (!NFAStack.empty())
    {
        NFAnode *currentNode = NFAStack.top();
        NFAStack.pop();
        visited_nodes.insert(currentNode);

        for (NFAedge edge : currentNode->edges)
        {
            string transition_type = edge.type;
            NFAnode *nextNode = edge.next;

            // 记录状态转换信息
            statusTable[currentNode->id].state_reachable[transition_type].insert(nextNode->id);

            // 如果下一个状态未被访问，将其加入堆栈
            if (visited_nodes.find(nextNode) == visited_nodes.end())
            {
                NFAStack.push(nextNode);

                // 记录状态信息
                statusTableNode nextStatus;
                nextStatus.id = nextNode->id;
                if (nextNode->isInit)
                {
                    nextStatus.flag += '-'; // -表示初态
                    initNFAstatus.insert(nextStatus.id);
                }
                else if (nextNode->isFinal)
                {
                    nextStatus.flag += '+'; // +表示终态
                    finalNFAstatus.insert(nextStatus.id);
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
        case 92: // 反斜杠，转义字符处理
            if (!regex.empty())
            {
                char next_char = regex.front();
                regex.erase(regex.begin()); // 移除处理过的字符
                switch (next_char)
                {
                case '(':
                case ')':
                case '|':
                case '.':
                case '?':
                case '*':
                case '+':
                case '[':
                case ']':
                    // 将转义字符作为普通字符处理
                    NFA nfa = NFA_basic(next_char);
                    NFAStack.push(nfa);
                    break;
                default:
                    // 如果转义字符无效，则报错
                    cout << "Invalid escaped character: \\" << next_char << endl;
                    exit(-1);
                }
            }
            else
            {
                // 如果反斜杠后没有字符，则报错
                cout << "Invalid escaped character at the end of the regex." << endl;
                exit(-1);
            }
            break;
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

                if (op == '|')
                {
                    // 处理并构建"|"运算符
                    NFA nfa2 = NFAStack.top();
                    NFAStack.pop();
                    NFA nfa1 = NFAStack.top();
                    NFAStack.pop();

                    // 创建新的NFA，表示nfa1 | nfa2
                    NFA nfa_result = NFA_alternation(nfa1, nfa2);
                    NFAStack.push(nfa_result);
                }
                else if (op == '.')
                {
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

        case '|':
        case '.':
            // 处理运算符 | 和 .
            while (!opStack.empty() && (opStack.top() == '|' || opStack.top() == '.') &&
                   prec(opStack.top()) >= prec(c))
            {
                char op = opStack.top();
                opStack.pop();

                // 处理栈顶运算符，构建NFA图，并将结果入栈
                if (op == '|')
                {
                    // 处理并构建"|"运算符
                    NFA nfa2 = NFAStack.top();
                    NFAStack.pop();
                    NFA nfa1 = NFAStack.top();
                    NFAStack.pop();

                    // 创建新的NFA，表示nfa1 | nfa2
                    NFA nfa_result = NFA_alternation(nfa1, nfa2);
                    NFAStack.push(nfa_result);
                }
                else if (op == '.')
                {
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
            opStack.push(c);
            break;
        case '?':
        case '*':
            // 处理闭包运算符 ? + *
            // 从NFAStack弹出NFA，应用相应的闭包操作，并将结果入栈
            if (!NFAStack.empty())
            {
                NFA nfa = NFAStack.top();
                NFAStack.pop();
                if (c == '?')
                {
                    // 处理 ?
                    NFA nfa_result = NFA_optional(nfa);
                    NFAStack.push(nfa_result);
                }
                else if (c == '*')
                {
                    // 处理 *
                    NFA nfa_result = NFA_kleene_star(nfa);
                    NFAStack.push(nfa_result);
                }
            }
            else
            {
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
            if (NFAStack.size() < 2)
            {
                cout << "Error! Not enough  " << op;
                exit(-1);
            }

            NFA nfa2 = NFAStack.top();
            NFAStack.pop();
            NFA nfa1 = NFAStack.top();
            NFAStack.pop();

            if (op == '|')
            {
                // 创建新的 NFA，表示 nfa1 | nfa2
                NFA nfa_result = NFA_alternation(nfa1, nfa2);
                NFAStack.push(nfa_result);
            }
            else if (op == '.')
            {
                // 创建新的 NFA，表示 nfa1 . nfa2
                NFA nfa_result = NFA_connect(nfa1, nfa2);
                NFAStack.push(nfa_result);
            }
        }
        else
        {
            cout << "Error! Unknown operator: " << op;

            exit(-1);
        }
    }

    // 最终的NFA图在nfaStack的顶部
    NFA result = NFAStack.top();
    cout << "NFA has been successfully built." << endl;

    createNFAStatusTable(result);
    cout << "NFA status table has been successfully built." << endl;

    return result;
}
/*----------NFA to DFA----------*/
// 判断是否含有初态终态，含有则返回对应字符串
string containsInitOrFinal(set<int> &statusSet)
{
    string result = "";
    for (const int &element : initNFAstatus)
    {
        if (statusSet.count(element) > 0)
        {
            result += "-";
        }
    }

    for (const int &element : finalNFAstatus)
    {
        if (statusSet.count(element) > 0)
        {
            result += "+";
        }
    }

    return result;
}
// DFS实现epsilon闭包的计算
set<int> epsilonClosure(int id)
{
    set<int> epsilon_closure{id}; // 将id也包括到epsilon_closure,因为一个结点的epsilon闭包也包括本身
    // DFS实现epsilon闭包
    stack<int> stack;
    stack.push(id);
    while (!stack.empty())
    {

        int current = stack.top(); // 在循环中，不断从栈中弹出一个状态 current。
        stack.pop();

        set<int> temp = statusTable[current].state_reachable[EPSILON]; // 对于每个弹出的状态 current，获取其通过ε转移可达的状态集合temp，并遍历这个集合。
        for (auto t : temp)                                            // 对于每个可达状态 t，
            if (epsilon_closure.find(t) == epsilon_closure.end())      // 如果它不在 eepsilon_closure 中，就将其加入到 epsilon_closure 中，并将其入栈，以便后续继续搜索
            {
                {
                    epsilon_closure.insert(t);
                    stack.push(t);
                }
            }
    }
    return epsilon_closure;
}

set<int> otherClosure(int id, string str)
{
    set<int> other_result;
    set<int> processed;
    // DFS实现其他字符闭包
    stack<int> stack;
    stack.push(id);

    while (!stack.empty())
    {
        int current = stack.top();
        stack.pop(); // 在循环中，不断从栈中弹出一个状态 current。
        // 对于每个弹出的状态 current：
        if (processed.find(current) != processed.end()) // 如果 current 已经被处理过（在processed集合end之前，find到了），则跳过该状态（continue），继续处理下一个状态；
            continue;

        processed.insert(current); // 否则，将 current 标记为已处理。（插入processed集合）

        set<int> temp = statusTable[current].state_reachable[str]; // 获取状态 current 在输入字符 ch 下可以到达的状态集合 otherClosure。
        for (auto t : temp)                                        // 遍历 otherClosure 中的每个状态 o：对于每个可达状态 o，调用 epsilonClosure(o) 计算其ε闭包，并将得到的闭包中的所有状态加入到 other_result 中。
        {
            auto tmp = epsilonClosure(t);
            other_result.insert(tmp.begin(), tmp.end());
            stack.push(t); // 将状态 o 入栈，以便后续继续搜索。
        }
    }
    // 当栈为空时，表示所有通过输入字符 ch 可以到达的状态都已经被处理，函数返回 other_result，其中包含了其他闭包中的所有状态。
    return other_result;
}
int main()
{
    return 0;
}
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
const char EPSILON = '~';
// 结点数量统计
int node_count;
// 全局字符统计
set<char> NFAChar;
set<char> DFAChar;
struct NFAnode;
struct NFAedge // NFA边
{
    char type;
    NFAnode *next;
};
struct NFAnode // NFA结点
{
    int id;      // 结点编号
    bool isInit; // 是否为初态
    bool isAC;   // 是否为末态
    vector<NFAedge> edges;
    NFAnode()
    {
        id = node_count++;
        isInit = false;
        isAC = false;
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
    string flag;                         // 标记初态还是终态
    int id;                              // 唯一id值
    map<char, set<int>> state_reachable; // 对应字符能到达的状态
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
struct DFANode
{
    string flag;                     // 是否包含终态（+）或初态（-）
    set<int> nfaStates;              // 该DFA状态包含的NFA状态的集合
    map<char, set<int>> transitions; // 字符到下一状态的映射
    DFANode()
    {
        flag = "";
    }
};
// dfa状态去重集
set<set<int>> dfaStatusSet;

// dfa最终结果
vector<DFANode> dfaTable;

// 下面用于DFA最小化
//  dfa终态集合
set<int> dfaACStatusSet;
// dfa非终态集合
set<int> dfaNotACStatusSet;
// set对应序号MAP
map<set<int>, int> dfaToNumberMap;
int startStatus;

// dfa最小化节点
struct DFAMinNode
{
    string flag; // 是否包含终态（+）或初态（-）
    int id;
    map<char, int> transitions; // 字符到下一状态的映射
    DFAMinNode()
    {
        flag = "";
    }
};

vector<DFAMinNode> dfaMinTable;

// 用于分割集合
vector<set<int>> divideVector;
// 存下标
map<int, int> dfaMinMap;

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
    end->isAC = true;

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
    nfa_a.end->isAC = false;
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
    end->isAC = true;

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
    nfa_a.end->isAC = false;

    NFAedge edge4;
    edge4.type = EPSILON;
    edge4.next = end;
    nfa_b.end->edges.push_back(edge4);
    nfa_b.end->isAC = false;

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
    end->isAC = true;

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
    nfa_a.end->isAC = false;

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
    end->isAC = true;

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
    nfa_a.end->isAC = false;

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
    end->isAC = true;

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
    nfa_a.end->isAC = false;

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
            char transition_type = edge.type;
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
                else if (nextNode->isAC)
                {
                    nextStatus.flag += '+'; // +表示终态
                    finalNFAstatus.insert(nextStatus.id);
                }
                statusTable[nextNode->id] = nextStatus;
                // 记录插入顺序（排除终态）
                if (!nextNode->isAC)
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

// 测试输出NFA状态转换表程序（debug使用）
// void printStatusTable()
// {
//     // 打印状态表按照插入顺序
//     for (int id : insertionOrder)
//     {
//         const statusTableNode &node = statusTable[id];
//         cout << "Node ID: " << node.id << ", Flag: " << string(node.flag) << "\n";

//         for (const auto &entry : node.state_reachable)
//         {
//             char transitionChar = entry.first;
//             const std::set<int> &targetStates = entry.second;

//             cout << "  Transition: " << transitionChar << " -> {";
//             for (int targetState : targetStates)
//             {
//                 cout << targetState << " ";
//             }
//             cout << "}\n";
//         }
//     }
// }
/*----------NFA to DFA----------*/
// 判断是否含有初态终态，含有则返回对应字符串
string containsInitOrAC(set<int> &statusSet)
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

set<int> otherClosure(int id, char c)
{
    set<int> other_clousure;
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

        set<int> temp = statusTable[current].state_reachable[c]; // 获取状态 current 在输入字符 c 下可以到达的状态集合 otherClosure。
        for (auto t : temp)                                      // 遍历 otherClosure 中的每个状态 t：对于每个可达状态 t，调用 epsilonClosure(t) 计算其ε闭包，并将得到的闭包中的所有状态加入到 other_result 中。
        {
            auto t_epsilonClousure = epsilonClosure(t);
            other_clousure.insert(t_epsilonClousure.begin(), t_epsilonClousure.end());
            stack.push(t); // 将状态 t 入栈，以便后续继续搜索。
        }
    }
    // 当栈为空时，表示所有通过输入字符 ch 可以到达的状态都已经被处理，函数返回 other_closure，其中包含了其他闭包中的所有状态。
    return other_clousure;
}

void NFAtoDFA(NFA &nfa)
{
    // 1.NFA中初态等价合并
    // 2.从初态开始非ε转换
    /*-----初始化DFA的起始状态-----*/
    int dfaStatusCount = 1;   // dfa状态数计数器
    auto start = nfa.start;   // 获得NFA图的起始位置
    auto startId = start->id; // 获得起始编号

    DFANode startDFANode;
    startDFANode.nfaStates = epsilonClosure(startId);             // 初始闭包
    startDFANode.flag = containsInitOrAC(startDFANode.nfaStates); // 判断初始闭包是否包含初态和终态

    deque<set<int>> newStatus{}; // 新状态双向队列
    // 将初始状态的ε闭包与对应的编号存入 dfaToNumberMap 中，并将初始状态的编号记录为 startStatus。
    dfaToNumberMap[startDFANode.nfaStates] = dfaStatusCount;
    startStatus = dfaStatusCount;

    // 判断初始状态的ε闭包是否包含终态，如果是，则将其编号加入到终态集合 dfaACStatusSet 中；否则，将其编号加入到非终态集合 dfaNotACStatusSet 中，并递增 dfaStatusCount。
    if (containsInitOrAC(startDFANode.nfaStates).find("+") != string::npos)
    {
        dfaACStatusSet.insert(dfaStatusCount);
        dfaStatusCount++;
    }
    else
    {
        dfaNotACStatusSet.insert(dfaStatusCount);
        dfaStatusCount++;
    }

    // 针对起始状态 startDFANode 的每个输入字符 ch，进行遍历
    for (auto ch : DFAChar)
    {
        set<int> ch_closure{}; // 用于存储当前字符ch的闭包

        // 计算起始状态的输入字符 ch 下的其他字符闭包，并将结果存储在 ch_closure 中。
        for (auto c : startDFANode.nfaStates)
        {
            set<int> tmp = otherClosure(c, ch);
            ch_closure.insert(tmp.begin(), tmp.end());
        }

        // 若计算出当前字符ch闭包为空，直接进入下一个字符
        if (ch_closure.empty())
        {
            continue;
        }
        // 若不为空，进入下面的处理
        int size_before = dfaStatusSet.size();
        dfaStatusSet.insert(ch_closure); // 将计算得到的 ch_closure 加入到 DFA 状态集合 dfaStatusSet 中
        int size_after = dfaStatusSet.size();
        startDFANode.transitions[ch] = ch_closure; // 不管一不一样都是该节点这个字符的状态
        // 如果大小不一样，证明是新状态
        if (size_after > size_before)
        {
            // 将其加入到队列 newStatus 中，更新状态编号
            dfaToNumberMap[ch_closure] = dfaStatusCount;
            newStatus.push_back(ch_closure);
            if (containsInitOrAC(ch_closure).find("+") != string::npos)
            {
                dfaACStatusSet.insert(dfaStatusCount);
                dfaStatusCount++;
            }
            else
            {
                dfaNotACStatusSet.insert(dfaStatusCount);
                dfaStatusCount++;
            }
        }
    }
    // 将起始状态 startDFANode 在每个输入字符 ch 下的转移关系存储在转移表 startDFANode.transitions[ch] 中
    dfaTable.push_back(startDFANode);

    // 对后面的新状态进行不停遍历
    while (!newStatus.empty())
    {
        // 拿出一个新状态
        set<int> ns = newStatus.front();
        newStatus.pop_front();
        DFANode DFANode;
        DFANode.nfaStates = ns; // 该节点状态集合
        DFANode.flag = containsInitOrAC(ns);

        for (auto ch : DFAChar)
        {

            set<int> ch_closure{};
            for (auto c : ns)
            {
                set<int> tmp = otherClosure(c, ch);
                ch_closure.insert(tmp.begin(), tmp.end());
            }
            if (ch_closure.empty()) // 如果这个闭包是空集没必要继续下去了
            {
                continue;
            }

            int size_before = dfaStatusSet.size(); // 记录更新状态集合前的大小
            dfaStatusSet.insert(ch_closure);       // 更新状态集合
            int size_after = dfaStatusSet.size();  // 记录更新状态集合后的大小
            // 若计算出当前字符ch闭包为空，直接进入下一个字符
            DFANode.transitions[ch] = ch_closure;

            // 如果大小不一样，证明是新状态
            if (size_after > size_before)
            {
                dfaToNumberMap[ch_closure] = dfaStatusCount;
                newStatus.push_back(ch_closure);
                if (containsInitOrAC(ch_closure).find("+") != string::npos)
                {
                    dfaACStatusSet.insert(dfaStatusCount++);
                }
                else
                {
                    dfaNotACStatusSet.insert(dfaStatusCount++);
                }
            }
        }
        dfaTable.push_back(DFANode);
    }

    // dfa debug
    // printDfaTable(dfaTable);
}
/*----------DFA Minimization----------*/
// 判断是否含有初态终态，含有则返回对应字符串
string minContainInitOrAC(set<int> &statusSet)
{
    string result = "";
    if (statusSet.count(startStatus) > 0)

    {
        result += "-";
    }

    for (const int &element : dfaACStatusSet)
    {
        if (statusSet.count(element) > 0)
        {
            result += "+";
            break; // 可能会有多个终态同时包含，我们只要一个
        }
    }

    return result;
}

// 根据字符 ch 将状态集合 node 分成两个子集合
void splitSet(int i, char ch)
{
    set<int> result;
    auto &node = divideVector[i];
    int s = -2;

    for (auto state : node)
    {
        int thisNum;
        if (dfaTable[state - 1].transitions.find(ch) == dfaTable[state - 1].transitions.end())
        {
            thisNum = -1; // 空集
        }
        else
        {
            // 根据字符 ch 找到下一个状态
            int next_state = dfaToNumberMap[dfaTable[state - 1].transitions[ch]];
            thisNum = dfaMinMap[next_state]; // 这个状态的下标是多少
        }

        if (s == -2) // 初始下标
        {
            s = thisNum;
        }
        else if (thisNum != s) // 如果下标不同，就是有问题，需要分出来
        {
            result.insert(state);
        }
    }

    // 删除要删除的元素
    for (int state : result)
    {
        node.erase(state);
    }

    // 都遍历完了，如果result不是空，证明有新的，加入vector中
    if (!result.empty())
    {
        divideVector.push_back(result);
        // 同时更新下标
        for (auto a : result)
        {
            dfaMinMap[a] = divideVector.size() - 1;
        }
    }
}

void DFAminimize()
{
    divideVector.clear();
    dfaMinMap.clear();

    // 存入非终态、终态集合
    if (dfaNotACStatusSet.size() != 0)
    {
        divideVector.push_back(dfaNotACStatusSet);
    }
    // 初始化map
    for (auto t : dfaNotACStatusSet)
    {
        dfaMinMap[t] = divideVector.size() - 1;
    }

    divideVector.push_back(dfaACStatusSet);

    for (auto t : dfaACStatusSet)
    {
        dfaMinMap[t] = divideVector.size() - 1;
    }

    // 当flag为1时，一直循环
    int continueFlag = 1;

    while (continueFlag)
    {
        continueFlag = 0;
        int size1 = divideVector.size();

        for (int i = 0; i < size1; i++)
        {

            // 逐个字符尝试分割状态集合
            for (char ch : DFAChar)
            {
                splitSet(i, ch);
            }
        }
        int size2 = divideVector.size();
        if (size2 > size1)
        {
            continueFlag = 1;
        }
    }

    for (int dfaMinCount = 0; dfaMinCount < divideVector.size(); dfaMinCount++)
    {
        auto &v = divideVector[dfaMinCount];
        DFAMinNode d;
        d.flag = minContainInitOrAC(v);
        d.id = dfaMinCount;
        // 逐个字符
        for (char ch : DFAChar)
        {
            if (v.size() == 0)
            {
                d.transitions[ch] = -1; // 空集特殊判断
                continue;
            }
            int i = *(v.begin()); // 拿一个出来
            if (dfaTable[i - 1].transitions.find(ch) == dfaTable[i - 1].transitions.end())
            {
                d.transitions[ch] = -1; // 空集特殊判断
                continue;
            }
            int next_state = dfaToNumberMap[dfaTable[i - 1].transitions[ch]];
            int thisNum = dfaMinMap[next_state]; // 这个状态下标
            d.transitions[ch] = thisNum;
        }
        dfaMinTable.push_back(d);
    }

    // 输出 dfaMinTable
    for (const DFAMinNode &node : dfaMinTable)
    {
        cout << "State " << node.id << ":";
        cout << "Flag: " << string(node.flag);

        for (const auto &entry : node.transitions)
        {
            cout << entry.first << " -> " << entry.second;
        }
    }

    cout << "DFA minimazation has been successfully completed!";
}

int main()
{
    return 0;
}
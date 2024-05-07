![images](cover_ex2.jpg)
## 一、实验内容
设计一个应用软件，以实现将正则表达式-->NFA--->DFA-->DFA最小化-->词法分析程序(选做内容)

二、必做实验内容及要求：
 （1）正则表达式应该可以支持命名操作，运算符号有： 转义符号`\`、 连接、 选择`|`、  闭包`*`、  正闭包`+`、  `[]`、 可选`?`、 括号`( )`。
 （2）要提供一个源程序编辑界面，让用户输入一行（一个）或多行（多个）正则表达式（可保存、打开正则表达式文件。
 （3）需要提供窗口以便用户可以查看转换得到的NFA（用状态转换表呈现即可）。
 （4）需要提供窗口以便用户可以查看转换得到的DFA（用状态转换表呈现即可）。
 （5）需要提供窗口以便用户可以查看转换得到的最小化DFA（用状态转换表呈现即可）。
 （6）要求应用程序为Windows界面。
 （7）书写完善的软件文档。
## 二、实验目的
做这个实验可以达到以下几个目的：

1. **理解正则表达式的基本概念和运算规则**：通过实践操作，可以更深入地理解正则表达式的语法结构、运算符号以及它们之间的关系，如连接、选择、闭包等。

2. **掌握有限自动机的转换原理**：通过将正则表达式转换成NFA和DFA，并对DFA进行最小化处理，可以加深对有限自动机的理解，掌握其转换原理和算法。

3. **提高编程实现能力**：实现这个应用软件需要学生具备一定的编程能力，包括用户界面设计、正则表达式解析、NFA和DFA的转换算法实现等，这可以帮助提高编程实践能力。

4. **加深对编译原理的理解**：词法分析是编译原理中的重要内容，通过这个实验，可以深入了解词法分析的原理和实现方法，为后续学习编译原理提供扎实的基础。

## 三、实验文档
### 算法思路
-  数据存储结构
```C++
struct nfaNode // NFA图的结点
{
    int id; // 结点编号
    bool isStart;   // 是否为初态
    bool isAC; // 是否为末态（接受态Accect）
    vector<nfaEdge> edges;
    nfaNode()
    {
        id = nodeCount++;
        isStart = false;
        isAC = false;
    }
};
struct nfaEdge // NFA图的边
{
    char trans;
    nfaNode* next;
};
```
值得注意的是，由于初期设计的疏忽，只考虑了每条边上的转换只消耗一个字符，而没有考虑到演示中赋值的情况。因此在本程序中，变量的命名只能命名为一个字符。
```C++

struct dfaNode// DFA图的节点
{
    string flag; // 是否包含终态（+）或初态（-）
    set<int> nfaStates; // 该DFA状态包含的NFA状态的集合
    map<char, set<int>> transitions; // 字符到下一状态的映射
    dfaNode() {
        flag = "";
    }
};
```
```C++
struct dfaMinNode   // DFA最小化节点
{
    string flag; // 是否包含终态（+）或初态（-）
    int id;
    map<char, int> transitions; // 字符到下一状态的映射
    dfaMinNode() {
        flag = "";
    }
};
```
```C++
int nodeCount = 0;// 全局结点计数器

const char EPSILON = '~';

// 全局字符统计
set<char> nfaChar;
set<char> dfaChar;

unordered_map<int, statusTableNode> statusTable;// 全局存储状态转换表

vector<int> insertionOrder;// statusTable插入顺序记录，方便后续输出

set<int> startNFAstatus;

set<int> acNFAstatus;

unordered_map<string, string> regexMap;//用于存储变量名和对应的正则表达式字符串

set<set<int>> dfaStatusSet;// dfa状态去重集

vector<dfaNode> dfaTable;// dfa最终结果

set<int> dfaacStatusSet;// dfa终态集合

set<int> dfaNotacStatusSet;// dfa非终态集合

map<set<int>, int> dfaToNumberMap;// set对应序号MAP

vector<dfaMinNode> dfaMinTable;

vector<set<int>> divideVector;// 用于分割集合

map<int, int> dfaMinMap;// 存下标

```

- 正则表达式转NFA
1. 正则表达式处理
    - 多行处理
    对于给出多行的正则表达式，我们认为此时出现赋值运算。如：
    ```
    digit=[0-9]
    letter=[a-zA-Z]
    id=letter(letter|digit)*
    ```
    核心思想在于：**需要建立map结构，对每一条正则表达式的变量名（`digit`、 `letter`、 `_id`）和其对应的正则表达式建立映射。再对每一个变量名进行遍历，若其出现了在某一条正则表达式中，判断其对应的变量名是否以下划线`_`开始。若是，则对该条正则表达式进行替换处理：里面引用的所有变量名将被其对应的正则表达式所替换。**
    <h6>但为了展示要求演示范例中的效果，本程序没有进行替换操作。</h6>

    - 连接运算
    对于所有的连接，都用`.`进行连接。
    - 字符范围（不考虑中括号中出现不连续的情况，比如`[a|d|c|b]`）
    - 字符范围`[abc]`可写成`a|b|c`
    - 字符范围`[a-d]`可写成`a|b|c|d`

2. NFA机器构造
连接、 选择`|`、  闭包`*`、  正闭包`+`、  可选`?`五种运算需要构造机器，每种运算都需要一个函数，其输入是需要运算的NFA图、输出是运算后的NFA图。下面将以可选运算给出核心算法。
```C++
NFA NFA_optional(NFA nfa_a)
{
    NFA NFA_optional(NFA nfa1)
{
    nfaNode* start = new nfaNode();
    nfaNode* ac = new nfaNode();

    start->isStart = true;
    ac->isAC = true;

    // 新初态和nfa1的初态进行EPOSILON连接
    nfaEdge edge1;
    edge1.trans = EPSILON;
    edge1.next = nfa1.start;
    start->edges.push_back(edge1);
    nfa1.start->isStart = false;

    // nfa1的终态和新末态进行EPOSILON连接
    nfaEdge edge2;
    edge2.trans = EPSILON;
    edge2.next = ac;
    start->edges.push_back(edge2);
    nfa1.ac->isAC = false;

    // 可选运算的实现：新初态和新末态进行EPOSILON连接
    nfaEdge edge3;
    edge3.trans = EPSILON;
    edge3.next = ac;
    nfa1.ac->edges.push_back(edge3);

    NFA nfa(start, ac);
    return nfa;
}
}
```
核心思想在于：**新建NFA结点，进行epsilon连接实现运算，并根据运算的性质更新初态和末态（通常来讲，新建的两个结点就会成为新的初态和末态）。**
1. 正则表达式转NFA算法
核心思想在于：**对于传入的正则表达式，for循环遍历每一个字符，用switch结构枚举，结合运算优先级，处理会出现的每一种情况。**
    - 转义字符处理
        若字符为`\`，则判断其下一个字符是否为运算符。若是，直接将该运算符视作普通字符，进行处理；若否，则将字符`\`视作普通字符，进行处理。
    - 常规运算处理
      - 当遇到 `(`时，将其压入运算符栈` opStack`。
      - 当遇到 `)` 时，弹出 `opStack` 中的运算符，根据运算符构建对应的NFA图，并将结果入栈 nfaStack，直到遇到 `(`。
      - `|` 或 `.`运算符时，判断其与栈顶运算符的优先级，若优先级较高则先处理栈顶运算符，直到栈为空或遇到优先级更低的运算符，然后将当前运算符入栈。
      - 这一步确保了处理运算符时的正确顺序，遵循了正则表达式的运算优先级。
    - 普通字符处理
    构建一个最简单的NFA：消耗该字符进入下一个状态。
```C++
for (int i = 0; i < regex.size(); i++)
    {
        NFA tmp;
        switch (regex[i])
        {

        case '\\':
            if (i + 1 < regex.size()) {
                char nextChar = regex[i + 1];
                if (nextChar == '+' || nextChar == '?' || nextChar == '*' || nextChar == '(' || nextChar == ')' || nextChar == '[' || nextChar == ']' || nextChar == '|' || nextChar == '.') {
                    tmp = NFA_basic(nextChar); // 创建基本的字符NFA
                    nfaStack.push(tmp);
                    i++; // 跳过已经处理的转义字符
                } else {
                    // 如果后面的字符不是特殊字符，则将反斜杠作为普通字符处理
                    tmp = NFA_basic('\\'); // 创建基本的字符NFA
                    nfaStack.push(tmp);
                }
            } else {
                // 如果反斜杠是字符串的最后一个字符，则将其视为普通字符处理
                tmp = NFA_basic('\\'); // 创建基本的字符NFA
                nfaStack.push(tmp);
            }
            break;
        case ' ': // 空格跳过
            break;
        case '(':
            opStack.push(regex[i]);
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
                    NFA nfa2 = nfaStack.top();
                    nfaStack.pop();
                    NFA nfa1 = nfaStack.top();
                    nfaStack.pop();

                    // 创建新的NFA，表示nfa1 | nfa2
                    NFA resultNFA = NFA_alternation(nfa1, nfa2);
                    nfaStack.push(resultNFA);
                }
                else if (op == '.')
                {
                    // 处理并构建"."运算符
                    NFA nfa2 = nfaStack.top();
                    nfaStack.pop();
                    NFA nfa1 = nfaStack.top();
                    nfaStack.pop();

                    // 创建新的NFA，表示nfa1 . nfa2
                    NFA resultNFA = NFA_connect(nfa1, nfa2);
                    nfaStack.push(resultNFA);
                }
            }
            if (opStack.empty())
            {
                qDebug() << "Error！The brackets are not closed！";
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
                   prec(opStack.top()) >= prec(regex[i]))
            {
                char op = opStack.top();
                opStack.pop();

                // 处理栈顶运算符，构建NFA图，并将结果入栈
                if (op == '|')
                {
                    // 处理并构建"|"运算符
                    NFA nfa2 = nfaStack.top();
                    nfaStack.pop();
                    NFA nfa1 = nfaStack.top();
                    nfaStack.pop();

                    // 创建新的NFA，表示nfa1 | nfa2
                    NFA resultNFA = NFA_alternation(nfa1, nfa2);
                    nfaStack.push(resultNFA);
                }
                else if (op == '.')
                {
                    // 处理并构建"."运算符
                    NFA nfa2 = nfaStack.top();
                    nfaStack.pop();
                    NFA nfa1 = nfaStack.top();
                    nfaStack.pop();

                    // 创建新的 NFA，表示 nfa1 . nfa2
                    NFA resultNFA = NFA_connect(nfa1, nfa2);
                    nfaStack.push(resultNFA);
                }
            }
            opStack.push(regex[i]);
            break;
        case '?':
        case '*':
        case '+':
            // 处理闭包运算符 ? + *
            // 从nfaStack弹出NFA，应用相应的闭包操作，并将结果入栈
            if (!nfaStack.empty())
            {
                NFA nfa = nfaStack.top();
                nfaStack.pop();
                if (regex[i] == '?')
                {
                    // 处理 ?
                    NFA resultNFA = NFA_optional(nfa);
                    nfaStack.push(resultNFA);
                }
                else if (regex[i] == '*')
                {
                    // 处理 *
                    NFA resultNFA = NFA_kleene_star(nfa);
                    nfaStack.push(resultNFA);
                }
                else if (regex[i] == '+')
                {
                    // 处理 *
                    NFA resultNFA = NFA_kleene_plus(nfa);
                    nfaStack.push(resultNFA);
                }
            }
            else
            {
                qDebug() << "Error！NO NFA for closure calculation!";
                exit(-1);
            }
            break;
        default:
            // 处理字母字符
            NFA nfa = NFA_basic(regex[i]); // 创建基本的字符NFA
            nfaStack.push(nfa);
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
                qDebug() << "Regex syntax error!" << op << "！";
                exit(-1);
            }

            NFA nfa2 = nfaStack.top();
            nfaStack.pop();
            NFA nfa1 = nfaStack.top();
            nfaStack.pop();

            if (op == '|')
            {
                // 创建新的 NFA，表示 nfa1 | nfa2
                NFA resultNFA = NFA_alternation(nfa1, nfa2);
                nfaStack.push(resultNFA);
            }
            else if (op == '.')
            {
                // 创建新的 NFA，表示 nfa1 . nfa2
                NFA resultNFA = NFA_connect(nfa1, nfa2);
                nfaStack.push(resultNFA);
            }
        }
        else
        {
            qDebug() << "Error!Unknown syntax!" << op << "！";
            exit(-1);
        }
    }

    // 最终的NFA图在nfaStack的顶部
    NFA result = nfaStack.top();
    //// qDebug() << "NFA图构建完毕" << endl;

    createNFAStatusTable(result);
    //// qDebug() << "状态转换表构建完毕" << endl;

    return result;
}
```
- NFA转DFA
1. epsilon闭包计算
```C++
// DFS实现epsilon闭包的计算
// DFS实现epsilon闭包的计算
set<int> epsilonClosure(int id)
{
    set<int> epsilon_closure{ id };// 将id也包括到epsilon_closure,因为一个结点的epsilon闭包也包括本身
    stack<int> stack;
    stack.push(id);

    while (!stack.empty())
    {
        int current = stack.top();
        stack.pop();

        set<int> eClosure = statusTable[current].state_reachable[EPSILON];// 对于每个弹出的状态 current，获取其通过ε转移可达的状态集合temp，并遍历这个集合。
        for (auto t : eClosure)                             // 对于每个可达状态 t，
        {
            if (epsilon_closure.find(t) == epsilon_closure.end())           // 如果它不在 eepsilon_closure 中，就将其加入到 epsilon_closure 中，并将其入栈，以便后续继续搜索
            {
                epsilon_closure.insert(t);
                stack.push(t);
            }
        }
    }

    return epsilon_closure;
}
```
核心思想在于：**将NFA结构视作图，则每个epsilon闭包就是该结点可以通过权值为epsilon的边到达哪些点，存在一个集合set中（便于去重）。则可以用栈结构实现DFS实现。**
2. 非epsilon闭包计算
```C++
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
```
核心思想在于：**对给出的状态，看它可以根据给出的字符转移到哪些状态（这里与求epsilon闭包类似，用栈结构DFS实现），插入`other_closure`集合，并对该集合对每一个状态求epsilon闭包，继续插入`other_closure`集合。**
3. NFA转DFA算法
```C++
void NFAToDFA(NFA& nfa)
{

    /*-----初始化DFA的起始状态-----*/
    int dfaStatusCount = 1;// dfa状态数计数器
    auto start = nfa.start; // 获得NFA图的起始位置
    auto startId = start->id;   // 获得起始编号
    dfaNode startDFANode;
    startDFANode.nfaStates = epsilonClosure(startId); // 初始闭包
    startDFANode.flag = containStartOrAC(startDFANode.nfaStates); // 判断初始闭包是否包含初态和终态

    deque<set<int>> newStatus{};// 新状态双向队列
    // 将初始状态的ε闭包与对应的编号存入 dfaToNumberMap 中，并将初始状态的编号记录为 startStatus。
    dfaToNumberMap[startDFANode.nfaStates] = dfaStatusCount;
    startStaus = dfaStatusCount;

    // 判断初始状态的ε闭包是否包含终态，如果是，则将其编号加入到终态集合 dfaACStatusSet 中；否则，将其编号加入到非终态集合 dfaNotACStatusSet 中，并递增 dfaStatusCount。
    if (containStartOrAC(startDFANode.nfaStates).find("+") != string::npos)
    {
        dfaacStatusSet.insert(dfaStatusCount++);
    }
    else
    {
        dfaNotacStatusSet.insert(dfaStatusCount++);
    }
    // 针对起始状态 startDFANode 的每个输入字符 ch，进行遍历
    for (auto ch : dfaChar)
    {
        set<int> ch_closure{};// 用于存储当前字符ch的闭包
        // 计算起始状态的输入字符 ch 下的其他字符闭包，并将结果存储在 ch_closure 中。
        for (auto c : startDFANode.nfaStates)
        {
            set<int> tmp = otherClosure(c, ch);
            ch_closure.insert(tmp.begin(), tmp.end());
        }
        if (ch_closure.empty())          // 若计算出当前字符ch闭包为空，直接进入下一个字符
        {
            continue;
        }
        // 若不为空，进入下面的处理
        int size_before = dfaStatusSet.size();
        dfaStatusSet.insert(ch_closure);// 将计算得到的 ch_closure 加入到 DFA 状态集合 dfaStatusSet 中
        int size_after = dfaStatusSet.size();
        // 不管一不一样都是该节点这个字符的状态
        startDFANode.transitions[ch] = ch_closure;
        // 如果大小不一样，证明是新状态
        if (size_after > size_before)
        {
            // 将其加入到队列 newStatus 中，更新状态编号
            dfaToNumberMap[ch_closure] = dfaStatusCount;
            newStatus.push_back(ch_closure);
            if (containStartOrAC(ch_closure).find("+") != string::npos) {
                dfaacStatusSet.insert(dfaStatusCount++);
            }
            else
            {
                dfaNotacStatusSet.insert(dfaStatusCount++);
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
        dfaNode DFANode;
        DFANode.nfaStates = ns;  // 该节点状态集合
        DFANode.flag = containStartOrAC(ns);

        for (auto ch : dfaChar)
        {

            set<int> ch_closure{};
            for (auto c : ns)
            {
                set<int> tmp = otherClosure(c, ch);
                ch_closure.insert(tmp.begin(), tmp.end());
            }
            if (ch_closure.empty())  // 如果这个闭包是空集没必要继续下去了
            {
                continue;
            }
            int size_before = dfaStatusSet.size();
            dfaStatusSet.insert(ch_closure);
            int size_after = dfaStatusSet.size();
            // 若计算出当前字符ch闭包为空，直接进入下一个字符
            DFANode.transitions[ch] = ch_closure;
            // 如果大小不一样，证明是新状态
            if (size_after > size_before)
            {
                dfaToNumberMap[ch_closure] = dfaStatusCount;
                newStatus.push_back(ch_closure);
                if (containStartOrAC(ch_closure).find("+") != string::npos) {
                    dfaacStatusSet.insert(dfaStatusCount++);
                }
                else
                {
                    dfaNotacStatusSet.insert(dfaStatusCount++);
                }

            }

        }
        dfaTable.push_back(DFANode);

    }
}

```
核心思想在于：**利用epsilon闭包和转移闭包的概念，从给定的NFA中逐步构建DFA状态集合和转移表。首先确定DFA的起始状态，并根据其epsilon闭包判断是否为终态，然后针对每个DFA状态和输入字符，计算其在NFA中的转移闭包，并将结果加入到DFA状态集合中，构建转移表。通过不断扩展状态集合，直到没有新的状态加入，完成了NFA到DFA的转换过程。**
- DFA最小化
```C++
void DFAminimize()
{
    divideVector.clear();
    dfaMinMap.clear();

    // 存入非终态、终态集合
    if (dfaNotacStatusSet.size() != 0)
    {
        divideVector.push_back(dfaNotacStatusSet);
    }
    // 初始化map
    for (auto t : dfaNotacStatusSet)
    {
        dfaMinMap[t] = divideVector.size() - 1;
    }

    divideVector.push_back(dfaacStatusSet);

    for (auto t : dfaacStatusSet)
    {
        dfaMinMap[t] = divideVector.size() - 1;
    }

    int continueFlag = 1;

    while (continueFlag)
    {
        continueFlag = 0;
        int size1 = divideVector.size();

        for (int i = 0; i < size1; i++)
        {

            // 逐个字符尝试分割状态集合
            for (char ch : dfaChar)
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
        auto& v = divideVector[dfaMinCount];
        dfaMinNode d;
        d.flag = mincontainStartOrAC(v);
        d.id = dfaMinCount;
        // 逐个字符
        for (char ch : dfaChar)
        {
            if (v.size() == 0)
            {
                d.transitions[ch] = -1;   // 空集特殊判断
                continue;
            }
            int i = *(v.begin()); // 拿一个出来
            if (dfaTable[i - 1].transitions.find(ch) == dfaTable[i - 1].transitions.end())
            {
                d.transitions[ch] = -1;   // 空集特殊判断
                continue;
            }
            int next_state = dfaToNumberMap[dfaTable[i - 1].transitions[ch]];
            int thisNum = dfaMinMap[next_state];    // 这个状态下标
            d.transitions[ch] = thisNum;
        }
        dfaMinTable.push_back(d);
    }
}
```
核心思想在于：**使用等价关系将具有相同行为的DFA状态合并到同一个等价类中，从而减少状态数目，实现DFA的最小化。首先，将DFA状态划分为初始的终态和非终态等价类，然后通过迭代合并等价类的方式不断减少状态数目，最终构建最小化的DFA转移表。**




### 四、实验总结
通过这次实验，我熟悉了正则表达式转换成NFA和DFA的方法以及其算法的实现。同时熟练了对各类数据结构及其操作的使用。同时通过在维基百科查阅一些官方的说明，我尽量规范了一些变量名，这也使得我对自动机方面的一些术语有了初步的了解。
### 五、参考文献
- Michael Sipser, Introduction to the Theory of Computation. PWS, Boston. 1997. ISBN 0-534-95097-3. *（see section 1.2: Nondeterminism, pp.47–63.）*
- John E. Hopcroft and Jeffrey D. Ullman, Introduction to Automata Theory, Languages and Computation, Addison-Wesley Publishing, Reading Massachusetts, 1979. ISBN 0-201-02988-X. *（See chapter 2.）*
- https://en.wikipedia.org/wiki/Nondeterministic_finite_automaton
- https://en.wikipedia.org/wiki/Deterministic_finite_automaton
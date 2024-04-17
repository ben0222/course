### 一个运输公司每天有100吨的航空运输能力，公司每吨收空运费250美元.除了重量的限制外，由于飞机货舱的容积有限，公司每天只能运50000立方英尺。的货物、每天要运输的货物数量如下
$$
\begin{array}
{c}
\hline
	货物 & 重量（吨）& 体积（立方英尺/吨）\\
\hline
    1 & 30 & 550\\
    2 & 40 & 800\\
    3 & 50 & 400\\	               
\hline	          
\end{array}
$$  
### (a)采用有约束优化模型求使利润最大的每天航空运输的各种货物的吨数，利用拉格朗日乘子法求解。
### (b)计算每个约束的影子价格，解释它们的含义。
### (c)公司有能力对它的一些旧的飞机进行改装来增大货运区域的空间，每架飞机的改造要花费200000美元，可以增加2000立方英尺的容积.重量限制仍保持不变。假设飞机每年飞行250天，这些旧飞机剩余的使用寿命约为5年，在这种情况下，是否值得进行改装?有多少架飞机时才值得改装?

#### 模型假设
- 将利润定义为营业额与成本之差，由于成本未知，则求解利润最大等价于求解营业额最大。
- 飞机每天最多只能运输 50000 立方英尺的货物。
- 飞机每天最多只能运 100 吨货物。
#### 模型建立
#### 问题(a) 
##### 确定约束条件
**每日运力有限**：每天仅有100吨的运输能力，即每天三种货物的重量总和小于100吨。设三种货物的运输重量分别为\(m_1,m_2,m_3\)（单位：吨），则
\[m_1+ m_2+ m_3 < 100\]
**货场容积有限**：每天运输货物体积小于50000立方英尺，则
\[550m_1+ 800m_2+ 400m_3 < 50000\]
**运输需求有限**：每种货物都有自己的运输需求量，所运输的重量不能超过这个需求量。则，
\[
\begin{cases}
    0 \leq m_1 \leq 30 \\
    0 \leq m_2 \leq 40 \\
    0 \leq m_3 \leq 50 \\
\end{cases}
\]
##### 建立约束优化模型
设营业额为$W$，则目标函数为
\[W=(m_1+ m_2+ m_3) \times 250\]
约束条件为
\[
    s.t.
\begin{cases}
    m_1+ m_2+ m_3 < 100 \\
    550m_1+ 800m_2+ 400m_3 < 50000 \\
    0 \leq m_1 \leq 30 \\
    0 \leq m_2 \leq 40 \\
    0 \leq m_3 \leq 50 \\
\end{cases}
\]
#### 问题(c)
现在需建立改造飞机\( n \)架后，未来五年总收益的约束优化模型。
##### 确定约束条件
\[
    s.t.
\begin{cases}
    0 \leq m_1 \leq 30 \\
    0 \leq m_2 \leq 40 \\
    0 \leq m_3 \leq 50 \\
    N \geq 0 \\
\end{cases}
\]
##### 建立约束优化模型
设营业额为$Q$，则目标函数为
\[ Q = (m_1+ m_2+ m_3) \times 250 \times 250 \times 5 - 200000n\]
即
\[ Q = 312500(m_1+ m_2+ m_3) - 200000n\]

#### 模型求解
#### 问题(a) 
使用拉格朗日乘子法解决该问题。设拉格朗日乘子为 \(\lambda_1, \lambda_2, \lambda_3, \lambda_4, \lambda_5\)，则约束条件可改写如下：
\[
    s.t.
\begin{cases}
    m_1+ m_2+ m_3 - 100 + Z_1^2 = 0 \\
    550m_1+ 800m_2+ 400m_3 - 50000 + Z_2^2 = 0 \\
    m_1 - 30 + Z_3^2 = 0 \\
    m_2 - 40 + Z_4^2 = 0\\
    m_3 - 50 + Z_5^2 = 0\\
\end{cases}
\]
构建拉格朗日函数 \(L\) 如下：

\[
L(m, \lambda, Z) = (m_1 + m_2 + m_3) \times 250 + \lambda_1( m_1+ m_2+ m_3 - 100 + Z_1^2) \\ + \lambda_2(550m_1+ 800m_2+ 400m_3 - 50000 + Z_2^2) + \lambda_3(m_1 - 30 + Z_3^2) \\ + \lambda_4(m_2 - 40 + Z_4^2) + \lambda_5(m_3 - 50 + Z_5^2)
\]

求解 \(L\) 对所有自变量的偏导数，并令其等于零：

\[
\begin{align*}
\frac{\partial L}{\partial m_1} &= 250 + \lambda_1 + 550\lambda_2 + \lambda_3 = 0 \\
\frac{\partial L}{\partial m_2} &= 250 + \lambda_1 + 800\lambda_2 + \lambda_4 = 0 \\
\frac{\partial L}{\partial m_3} &= 250 + \lambda_1 + 400\lambda_2 + \lambda_5 = 0 \\
\frac{\partial L}{\partial \lambda_1} &= m_1+ m_2+ m_3 - 100 + Z_1^2 = 0 \\
\frac{\partial L}{\partial \lambda_2} &= 550m_1+ 800m_2+ 400m_3 - 50000 + Z_2^2 = 0\\
\frac{\partial L}{\partial \lambda_3} &= m_1 - 30 + Z_3^2 = 0\\
\frac{\partial L}{\partial \lambda_4} &= m_2 - 40 + Z_4^2 = 0\\
\frac{\partial L}{\partial \lambda_5} &= m_3 - 50 + Z_5^2 = 0\\
\frac{\partial L}{\partial Z_1} &= 2 \lambda_1 Z_1 = 0\\
\frac{\partial L}{\partial Z_2} &= 2 \lambda_2 Z_2 = 0\\
\frac{\partial L}{\partial Z_3} &= 2 \lambda_3 Z_3 = 0\\
\frac{\partial L}{\partial Z_4} &= 2 \lambda_4 Z_4 = 0\\
\frac{\partial L}{\partial Z_5} &= 2 \lambda_6 Z_5 = 0\\
\end{align*}
\]
解上述方程组得：

\[
\begin{align*}
\lambda_1 &= 0 \\
\lambda_2 &= - \frac{5}{16} \\
\lambda_3 &= -250 \\
\lambda_4 &= 0 \\
\lambda_5 &= -125 \\
Z_1 &= \sqrt{3.125}\\
Z_2 &= 0 \\
Z_3 &= 0 \\
Z_4 &= \sqrt{24.125} \\
Z_5 &= 0 \\
M_1 &= 30 \\
M_2 &= 16.875 \\
M_3 &= 50 \\
\end{align*}
\]
进而得到最优营业额 \(W = 24218.75\)。
#### 问题(b)
- 该公司每日运力为100吨，剩余$3.125$吨，影子价格0美元。
- 该公司每日货场体积无剩余，影子价格$0.3125$美元。
- 货物一影子价格$78.125$美元，这意味着在最大利润的运输方案下每增加$1$吨货物一，利润增加$78.125$美元。
- 货物二影子价格$0$美元，这意味着在最大利润的运输方案下货物二没有全部被运输。
- 货物三的影子价格为$125$美元，这意味着在最大利润的运输方案下每增加$1$吨货物三，利润增加$125$美元。

#### 问题(c)
使用拉格朗日乘子法解决该问题。设拉格朗日乘子为 \(\lambda_6, \lambda_7, \lambda_8, \lambda_9, \)，则约束条件可改写如下：
\[
    s.t.
\begin{cases}
    m_1 - 30 + Z_6^2 = 0 \\
    m_2 - 40 + Z_7^2 = 0\\
    m_3 - 50 + Z_8^2 = 0\\
    n - Z_9^2 = 0\\
\end{cases}
\]

构建拉格朗日函数 \(L\) 如下：

\[
L(m, \lambda, Z, n) = 312500(m_1+ m_2+ m_3) - 200000n + \lambda_6(m_1 - 30 + Z_6^2) + \lambda_7(m_2 - 40 + Z_7^2) \\ + \lambda_8(m_3 - 50 + Z_8^2) + \lambda_9(n - Z_9^2)
\]

求解 \(L\) 对所有自变量的偏导数，并令其等于零：

\[
\begin{align*}
\frac{\partial L}{\partial m_1} &= 312500 + \lambda_6 = 0 \\
\frac{\partial L}{\partial m_2} &= 312500 + \lambda_7 = 0 \\
\frac{\partial L}{\partial m_3} &= 312500 + \lambda_8 = 0 \\
\frac{\partial L}{\partial \lambda_6} &= m_1 + Z_6^2 - 30 = 0\\
\frac{\partial L}{\partial \lambda_7} &= m_2 + Z_7^2 - 40 = 0\\
\frac{\partial L}{\partial \lambda_8} &= m_3 + Z_8^2 - 50 = 0\\
\frac{\partial L}{\partial n} &= \lambda_9 - 200000 = 0\\
\frac{\partial L}{\partial \lambda_9} &= n - Z_9^2 = 0 \\
\frac{\partial L}{\partial Z_6} &= 2 \lambda_6 Z_6 = 0\\
\frac{\partial L}{\partial Z_7} &= 2 \lambda_7 Z_7 = 0\\
\frac{\partial L}{\partial Z_8} &= 2 \lambda_8 Z_8 = 0\\
\frac{\partial L}{\partial Z_9} &= 2 \lambda_9 Z_9 = 0\\
\end{align*}
\]
解上述方程组得：

\[
\begin{align*}
\lambda_6 &= -312500 \\
\lambda_7 &= -312500 \\
\lambda_8 &= -312500 \\
\lambda_9 &= 200000 \\
Z_6 &= 0\\
Z_7 &= 0 \\
Z_8 &= 0 \\
Z_9 &= \sqrt{1.25} \\
M_1 &= 30 \\
M_2 &= 20 \\
M_3 &= 50 \\
n &= 1.25\\
\end{align*}
\]


因此，应该改造1架。
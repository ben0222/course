import numpy as np
import matplotlib.pyplot as plt

# 定义本题模型
def function1(t, P0, P_star, r, t0):
    return (P0 - P_star) * np.exp(r * (t0 - t)) + P_star

# 定义指数增长模型
def function2(t, P0, r):
    return P0 * np.exp(r * t)

# 定义Logistic模型
def function3(t, P0, P_star, r):
    return P_star / (1 + (P_star / P0) * np.exp(-r * t))

# 生成时间点 t 的值
t_values = np.linspace(0, 10, 100)

# 计算第一个函数的 P(t) 值
P1_values = function1(t_values, P0=10, P_star=100, r=0.1, t0=0)

# 计算第二个函数的 P(t) 值
P2_values = function2(t_values, P0=10, r=0.1)

# 计算第三个函数的 P(t) 值
P3_values = function3(t_values, P0=10, P_star=100, r=0.1)

# 绘图
plt.figure(figsize=(10, 6))

plt.plot(t_values, P1_values, label='Model 1')
plt.plot(t_values, P2_values, label='Exponentional Growth Model')
plt.plot(t_values, P3_values, label='Logistic Model')

plt.xlabel('Time (t)')
plt.ylabel('P(t)')
plt.legend()

plt.grid(True)
plt.show()

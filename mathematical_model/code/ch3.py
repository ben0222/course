# 导入求解方程组所需的库
from scipy.optimize import fsolve

# 定义拉格朗日函数
def L(m1, m2, m3, l1, l2, l3, l4, l5):
    return (m1 + m2 + m3) * 250 + l1 * (100 - m1 - m2 - m3) + l2 * (50000 - 550 * m1 - 800 * m2 - 400 * m3) \
           + l3 * (-m1) + l4 * (-m2) + l5 * (-m3)

# 定义偏导数方程组
def equations(vars):
    m1, m2, m3, l1, l2, l3, l4, l5 = vars
    eq1 = 250 - l1 - 550 * l2 - l3
    eq2 = 250 - l1 - 800 * l2 - l4
    eq3 = 250 - l1 - 400 * l2 - l5
    eq4 = 100 - m1 - m2 - m3
    eq5 = 50000 - 550 * m1 - 800 * m2 - 400 * m3
    eq6 = -m1
    eq7 = -m2
    eq8 = -m3
    return [eq1, eq2, eq3, eq4, eq5, eq6, eq7, eq8]

# 使用 fsolve 函数求解方程组
m1, m2, m3, l1, l2, l3, l4, l5 = fsolve(equations, (0, 0, 0, 0, 0, 250, 1/550, 0))

# 计算最优营业额
W = (m1 + m2 + m3) * 250


# 输出结果
print("最优 m1:", m1)
print("最优 m2:", m2)
print("最优 m3:", m3)
print("最优营业额 W:", W)


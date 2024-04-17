from scipy.optimize import minimize

# 定义目标函数
def objective(m):
    return -(m[0] + m[1] + m[2]) * 250

# 定义约束条件方程组
def equations(vars):
    m1, m2, m3, l1, l2, l3, l4, l5 = vars
    eq1 = 250 - l1 - 550 * l2 - l3
    eq2 = 250 - l1 - 800 * l2 - l4
    eq3 = 250 - l1 - 400 * l2 - l5
    eq4 = 100 - m1 - m2 - m3
    eq5 = 50000 - (550*m1 + 800*m2 + 400*m3)
    return [eq1, eq2, eq3, eq4, eq5]

# 使用 minimize 函数求解方程组
sol = minimize(lambda vars: objective(vars[:3]), (0,0,0,0,0,250,1/550,0), constraints={'type': 'eq', 'fun': equations})

# 计算每个约束的影子价格
shadow_prices = [-sol.x[3], -sol.x[4], -sol.x[5], -sol.x[6], -sol.x[7]]

# 输出结果
print("最优 m1:", sol.x[0])
print("最优 m2:", sol.x[1])
print("最优 m3:", sol.x[2])
print("最优利润:", -sol.fun)

print("\n每个约束的影子价格：")
print("每天的运输能力约束影子价格:", shadow_prices[0])
print("货物体积约束影子价格:", shadow_prices[1])
print("单个货物的运输需求约束 m1 影子价格:", shadow_prices[2])
print("单个货物的运输需求约束 m2 影子价格:", shadow_prices[3])
print("单个货物的运输需求约束 m3 影子价格:", shadow_prices[4])

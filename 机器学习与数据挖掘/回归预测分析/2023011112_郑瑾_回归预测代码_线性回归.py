import json
from pathlib import Path

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from sklearn.datasets import fetch_california_housing


BASE_DIR = Path(__file__).resolve().parent
OUTPUT_DIR = BASE_DIR / "output"
FIG_DIR = OUTPUT_DIR / "figures"
TABLE_DIR = OUTPUT_DIR / "tables"

OUTPUT_DIR.mkdir(parents=True, exist_ok=True)
FIG_DIR.mkdir(parents=True, exist_ok=True)
TABLE_DIR.mkdir(parents=True, exist_ok=True)


def computeCost(X, y, theta):
    inner = np.power(((X * theta.T) - y), 2)
    return np.sum(inner) / (2 * len(X))


def gradientDescent(X, y, theta, alpha, iters):
    temp = np.matrix(np.zeros(theta.shape))
    parameters = int(theta.ravel().shape[1])
    cost = np.zeros(iters)

    for i in range(iters):
        error = (X * theta.T) - y
        for j in range(parameters):
            term = np.multiply(error, X[:, j])
            temp[0, j] = theta[0, j] - ((alpha / len(X)) * np.sum(term))

        theta = temp.copy()
        cost[i] = computeCost(X, y, theta)

    return theta, cost


print("=" * 80)
print("练习1：线性回归（按docx步骤，使用公开数据集）")
print("公开数据来源：Scikit-learn California Housing")
print("=" * 80)

# Step 0: 准备公开数据，写成与docx一致的 ex1data1/ex1data2 文件形式
print("\n[Step 0] 准备公开数据并导出 ex1data1.txt / ex1data2.txt")
housing = fetch_california_housing(as_frame=True)
df = housing.frame.copy()

# 单变量版本（类似 Population / Profit）
ex1data1 = df[["MedInc", "MedHouseVal"]].copy()
ex1data1_path = BASE_DIR / "ex1data1.txt"
ex1data1.to_csv(ex1data1_path, index=False, header=False)

# 多变量版本（类似 Size / Bedrooms / Price）
ex1data2 = df[["AveRooms", "HouseAge", "MedHouseVal"]].copy()
ex1data2_path = BASE_DIR / "ex1data2.txt"
ex1data2.to_csv(ex1data2_path, index=False, header=False)

print(f"ex1data1: {ex1data1_path}")
print(f"ex1data2: {ex1data2_path}")

# 1 实现简单示例函数
print("\n[Step 1] 实现简单示例函数（5x5单位对角矩阵）")
a = np.eye(5)
print(a)

# 2 单变量线性回归
print("\n[Step 2] 单变量线性回归")

# 2.1 绘制数据
print("[Step 2.1] 读取 ex1data1 并绘制散点图")
path = ex1data1_path
data = pd.read_csv(path, header=None, names=["Population", "Profit"])
print(data.head(5))

ax = data.plot(kind="scatter", x="Population", y="Profit", c="red", figsize=(12, 8))
ax.set_title("Step2.1 Scatter: Population vs Profit")
fig1 = ax.get_figure()
fig1.savefig(FIG_DIR / "step2_1_scatter.png", dpi=180, bbox_inches="tight")
plt.close(fig1)

# 2.2 梯度下降准备
print("[Step 2.2] 初始化参数并计算初始成本")
if "Ones" not in data.columns:
    data.insert(0, "Ones", 1)

cols = data.shape[1]
X = data.iloc[:, 0 : cols - 1]
y = data.iloc[:, cols - 1 : cols]
X = np.matrix(X.values)
y = np.matrix(y.values)

theta = np.matrix(np.array([0, 0]))
alpha = 0.01
iterations = 1500

initial_cost = float(computeCost(X, y, theta))
print(f"初始成本 J(theta=0): {initial_cost:.6f}")

# 2.2.4 梯度下降
print("[Step 2.2.4] 执行梯度下降")
g, cost = gradientDescent(X, y, theta, alpha, iterations)
final_cost = float(computeCost(X, y, g))
print(f"最终参数 theta: {g}")
print(f"最终成本: {final_cost:.6f}")

# 拟合曲线图
x = np.linspace(data.Population.min(), data.Population.max(), 100)
f = g[0, 0] + (g[0, 1] * x)

fig, ax = plt.subplots(figsize=(12, 8))
ax.plot(x, f, "b", label="Prediction")
ax.scatter(data.Population, data.Profit, c="red", label="Training Data")
ax.legend(loc=2)
ax.set_xlabel("Population")
ax.set_ylabel("Profit")
ax.set_title("Step2.2.4 Predicted Profit vs Population")
fig.savefig(FIG_DIR / "step2_2_fit_line.png", dpi=180, bbox_inches="tight")
plt.close(fig)

# 2.3 可视化成本函数
print("[Step 2.3] 绘制 cost 下降曲线")
fig, ax = plt.subplots(figsize=(12, 8))
ax.plot(np.arange(iterations), cost, "r")
ax.set_xlabel("Iterations")
ax.set_ylabel("Cost")
ax.set_title("Step2.3 Error vs Training Epoch")
fig.savefig(FIG_DIR / "step2_3_cost_curve.png", dpi=180, bbox_inches="tight")
plt.close(fig)

# 3 多变量线性回归（选做）
print("\n[Step 3] 多变量线性回归")

# 3.1 特征标准化
print("[Step 3.1] 读取 ex1data2 并标准化")
path2 = ex1data2_path
data2 = pd.read_csv(path2, header=None, names=["Size", "Bedrooms", "Price"])
print("标准化前前5行:")
print(data2.head())

data2 = (data2 - data2.mean()) / data2.std()
print("标准化后前5行:")
print(data2.head())
data2.head(20).to_csv(TABLE_DIR / "step3_1_normalized_head20.csv", index=False, encoding="utf-8-sig")

# 3.2 梯度下降
print("[Step 3.2] 多变量梯度下降")
if "Ones" not in data2.columns:
    data2.insert(0, "Ones", 1)

cols2 = data2.shape[1]
X2 = data2.iloc[:, 0 : cols2 - 1]
y2 = data2.iloc[:, cols2 - 1 : cols2]

X2 = np.matrix(X2.values)
y2 = np.matrix(y2.values)
theta2 = np.matrix(np.array([0, 0, 0]))

g2, cost2 = gradientDescent(X2, y2, theta2, alpha, iterations)
final_cost2 = float(computeCost(X2, y2, g2))
print(f"多变量最终参数 theta: {g2}")
print(f"多变量最终成本: {final_cost2:.6f}")

fig, ax = plt.subplots(figsize=(12, 8))
ax.plot(np.arange(iterations), cost2, "r")
ax.set_xlabel("Iterations")
ax.set_ylabel("Cost")
ax.set_title("Step3.2 Error vs Training Epoch")
fig.savefig(FIG_DIR / "step3_2_cost_curve_multi.png", dpi=180, bbox_inches="tight")
plt.close(fig)

summary = {
    "dataset": "California Housing (public)",
    "ex1data1_file": str(ex1data1_path),
    "ex1data2_file": str(ex1data2_path),
    "step2_initial_cost": initial_cost,
    "step2_final_theta": [float(g[0, 0]), float(g[0, 1])],
    "step2_final_cost": final_cost,
    "step3_final_theta": [float(g2[0, 0]), float(g2[0, 1]), float(g2[0, 2])],
    "step3_final_cost": final_cost2,
    "figure_dir": str(FIG_DIR),
}

with open(OUTPUT_DIR / "summary.json", "w", encoding="utf-8") as f:
    json.dump(summary, f, ensure_ascii=False, indent=2)

print("\n全部步骤执行完成。")
print(f"图像输出目录: {FIG_DIR}")
print(f"结果摘要: {OUTPUT_DIR / 'summary.json'}")

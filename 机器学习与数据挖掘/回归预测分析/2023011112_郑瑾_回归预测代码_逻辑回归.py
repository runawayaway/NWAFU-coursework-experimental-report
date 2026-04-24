import json
from pathlib import Path

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import scipy.optimize as opt
from sklearn.datasets import load_breast_cancer, load_digits
from sklearn.decomposition import PCA
from sklearn.preprocessing import StandardScaler


BASE_DIR = Path(__file__).resolve().parent
OUTPUT_DIR = BASE_DIR / "output"
FIG_DIR = OUTPUT_DIR / "figures"
TABLE_DIR = OUTPUT_DIR / "tables"

OUTPUT_DIR.mkdir(parents=True, exist_ok=True)
FIG_DIR.mkdir(parents=True, exist_ok=True)
TABLE_DIR.mkdir(parents=True, exist_ok=True)


def sigmoid(z):
    return 1 / (1 + np.exp(-z))


def cost(theta, X, y):
    theta = np.matrix(theta)
    X = np.matrix(X)
    y = np.matrix(y)
    eps = 1e-12
    h = sigmoid(X * theta.T)
    first = np.multiply(-y, np.log(h + eps))
    second = np.multiply((1 - y), np.log(1 - h + eps))
    return float(np.sum(first - second) / len(X))


def gradient(theta, X, y):
    theta = np.matrix(theta)
    X = np.matrix(X)
    y = np.matrix(y)
    parameters = int(theta.ravel().shape[1])
    grad = np.zeros(parameters)
    error = sigmoid(X * theta.T) - y

    for i in range(parameters):
        term = np.multiply(error, X[:, i])
        grad[i] = np.sum(term) / len(X)

    return grad


def predict(theta, X):
    theta = np.matrix(theta)
    X = np.matrix(X)
    probability = sigmoid(X * theta.T)
    return [1 if x >= 0.5 else 0 for x in np.asarray(probability).ravel()]


def costReg(theta, X, y, learningRate):
    theta = np.matrix(theta)
    X = np.matrix(X)
    y = np.matrix(y)
    eps = 1e-12
    h = sigmoid(X * theta.T)
    first = np.multiply(-y, np.log(h + eps))
    second = np.multiply((1 - y), np.log(1 - h + eps))
    reg = (learningRate / (2 * len(X))) * np.sum(np.power(theta[:, 1:theta.shape[1]], 2))
    return float(np.sum(first - second) / len(X) + reg)


def gradientReg(theta, X, y, learningRate):
    theta = np.matrix(theta)
    X = np.matrix(X)
    y = np.matrix(y)

    parameters = int(theta.ravel().shape[1])
    grad = np.zeros(parameters)

    error = sigmoid(X * theta.T) - y

    for i in range(parameters):
        term = np.multiply(error, X[:, i])

        if i == 0:
            grad[i] = np.sum(term) / len(X)
        else:
            grad[i] = (np.sum(term) / len(X)) + ((learningRate / len(X)) * theta[:, i])

    return grad


def map_feature(x1, x2, degree=5):
    out = [np.ones(x1.shape[0])]
    for i in range(1, degree):
        for j in range(0, i):
            out.append(np.power(x1, i - j) * np.power(x2, j))
    return np.stack(out, axis=1)


print("=" * 80)
print("练习2：逻辑回归（按docx步骤，公开数据集版）")
print("=" * 80)

# Step 0: 准备公开数据并导出 ex2data1/ex2data2
print("\n[Step 0] 准备公开数据并导出 ex2data1.txt / ex2data2.txt")

# ex2data1: 使用 breast_cancer 两个特征构造二分类数据
bc = load_breast_cancer(as_frame=True)
df1 = bc.frame[["mean radius", "mean texture"]].copy()
df1.columns = ["Exam 1", "Exam 2"]
# target: malignant=0, benign=1，直接作为 Admitted
admitted = bc.target.astype(int)
df1["Admitted"] = admitted
ex2data1_path = BASE_DIR / "ex2data1.txt"
df1.to_csv(ex2data1_path, index=False, header=False)

# ex2data2: digits -> PCA2维 -> 二分类
# 将目标定义为 digits>=5 为 1，其余为 0
_digits = load_digits(as_frame=True)
X_digits = _digits.data.values
y_digits = (_digits.target.values >= 5).astype(int)
X_scaled = StandardScaler().fit_transform(X_digits)
X_pca = PCA(n_components=2, random_state=42).fit_transform(X_scaled)
# 缩放到近似[-1,1]范围
x_min = X_pca.min(axis=0)
x_max = X_pca.max(axis=0)
X_norm = 2 * (X_pca - x_min) / (x_max - x_min) - 1

df2 = pd.DataFrame(X_norm, columns=["Test 1", "Test 2"])
df2["Accepted"] = y_digits
ex2data2_path = BASE_DIR / "ex2data2.txt"
df2.to_csv(ex2data2_path, index=False, header=False)

print(f"ex2data1: {ex2data1_path}")
print(f"ex2data2: {ex2data2_path}")

# 1 Logistic 回归
print("\n[Step 1] Logistic 回归")

# 1.1 数据可视化
print("[Step 1.1] 读取 ex2data1 并绘图")
path = ex2data1_path
data = pd.read_csv(path, header=None, names=["Exam 1", "Exam 2", "Admitted"])
print(data.head())
data.head(20).to_csv(TABLE_DIR / "step1_1_data_head20.csv", index=False, encoding="utf-8-sig")

positive = data[data["Admitted"].isin([1])]
negative = data[data["Admitted"].isin([0])]

fig, ax = plt.subplots(figsize=(12, 8))
ax.scatter(positive["Exam 1"], positive["Exam 2"], s=30, c="b", marker="o", label="Admitted")
ax.scatter(negative["Exam 1"], negative["Exam 2"], s=30, c="r", marker="x", label="Not Admitted")
ax.legend()
ax.set_xlabel("Exam 1 Score")
ax.set_ylabel("Exam 2 Score")
ax.set_title("Step1.1 Logistic Data Scatter")
fig.savefig(FIG_DIR / "step1_1_scatter.png", dpi=180, bbox_inches="tight")
plt.close(fig)

# 1.2.1 Sigmoid 函数测试
print("[Step 1.2.1] Sigmoid函数测试")
nums = np.arange(-10, 10, step=1)
fig, ax = plt.subplots(figsize=(12, 8))
ax.plot(nums, sigmoid(nums), "r")
ax.set_title("Step1.2.1 Sigmoid Curve")
fig.savefig(FIG_DIR / "step1_2_1_sigmoid.png", dpi=180, bbox_inches="tight")
plt.close(fig)

# 1.2.2 代价函数和梯度
print("[Step 1.2.2] 计算初始成本和梯度")
if "Ones" not in data.columns:
    data.insert(0, "Ones", 1)

cols = data.shape[1]
X = data.iloc[:, 0 : cols - 1]
y = data.iloc[:, cols - 1 : cols]

X = np.array(X.values)
y = np.array(y.values)
theta = np.zeros(3)

initial_cost = cost(theta, X, y)
initial_grad = gradient(theta, X, y)
print(f"初始成本: {initial_cost:.12f}")
print(f"初始梯度: {initial_grad}")

# 1.2.3 寻找最优参数
print("[Step 1.2.3] 使用TNC寻找最优参数")
result = opt.fmin_tnc(func=cost, x0=theta, fprime=gradient, args=(X, y))
theta_opt = np.matrix(result[0])
optimized_cost = cost(result[0], X, y)
print(f"优化结果theta: {result[0]}")
print(f"优化后成本: {optimized_cost:.12f}")

# 决策边界图
fig, ax = plt.subplots(figsize=(12, 8))
ax.scatter(positive["Exam 1"], positive["Exam 2"], s=30, c="b", marker="o", label="Admitted")
ax.scatter(negative["Exam 1"], negative["Exam 2"], s=30, c="r", marker="x", label="Not Admitted")

plot_x = np.array([data["Exam 1"].min(), data["Exam 1"].max()])
if abs(float(theta_opt[0, 2])) > 1e-10:
    plot_y = (-float(theta_opt[0, 0]) - float(theta_opt[0, 1]) * plot_x) / float(theta_opt[0, 2])
    ax.plot(plot_x, plot_y, "g-", label="Decision Boundary")

ax.legend()
ax.set_xlabel("Exam 1 Score")
ax.set_ylabel("Exam 2 Score")
ax.set_title("Step1.2.3 Decision Boundary")
fig.savefig(FIG_DIR / "step1_2_3_decision_boundary.png", dpi=180, bbox_inches="tight")
plt.close(fig)

# 1.2.4 评估
print("[Step 1.2.4] 评估逻辑回归")
predictions = predict(theta_opt, X)
truth = y.ravel().astype(int)
accuracy = float((np.array(predictions) == truth).mean() * 100)
print(f"accuracy = {accuracy:.2f}%")

# 2 正则化逻辑回归
print("\n[Step 2] 正则化逻辑回归")

# 2.1 数据可视化
print("[Step 2.1] 读取 ex2data2 并绘图")
path2 = ex2data2_path
data2 = pd.read_csv(path2, header=None, names=["Test 1", "Test 2", "Accepted"])
print(data2.head())
data2.head(20).to_csv(TABLE_DIR / "step2_1_data_head20.csv", index=False, encoding="utf-8-sig")

positive2 = data2[data2["Accepted"].isin([1])]
negative2 = data2[data2["Accepted"].isin([0])]

fig, ax = plt.subplots(figsize=(12, 8))
ax.scatter(positive2["Test 1"], positive2["Test 2"], s=20, c="b", marker="o", label="Accepted")
ax.scatter(negative2["Test 1"], negative2["Test 2"], s=20, c="r", marker="x", label="Rejected")
ax.legend()
ax.set_xlabel("Test 1 Score")
ax.set_ylabel("Test 2 Score")
ax.set_title("Step2.1 Regularized Logistic Data Scatter")
fig.savefig(FIG_DIR / "step2_1_scatter.png", dpi=180, bbox_inches="tight")
plt.close(fig)

# 2.2 特征映射
print("[Step 2.2] 特征映射")
degree = 5
x1 = data2["Test 1"]
x2 = data2["Test 2"]

data2_mapped = data2.copy()
data2_mapped.insert(3, "Ones", 1)

for i in range(1, degree):
    for j in range(0, i):
        data2_mapped["F" + str(i) + str(j)] = np.power(x1, i - j) * np.power(x2, j)

# 保持与docx一致
# 列：Accepted, Ones, F10 ...
data2_mapped.drop("Test 1", axis=1, inplace=True)
data2_mapped.drop("Test 2", axis=1, inplace=True)

print(f"特征映射后具有特征维数：{data2_mapped.shape[1]}")
data2_mapped.head(20).to_csv(TABLE_DIR / "step2_2_mapped_head20.csv", index=False, encoding="utf-8-sig")

# 2.3 代价函数和梯度
print("[Step 2.3] 正则化成本与梯度")
cols2 = data2_mapped.shape[1]
X2 = data2_mapped.iloc[:, 1:cols2]
y2 = data2_mapped.iloc[:, 0:1]

X2 = np.array(X2.values)
y2 = np.array(y2.values)
theta2 = np.zeros(X2.shape[1])
learningRate = 1

initial_cost_reg = costReg(theta2, X2, y2, learningRate)
initial_grad_reg = gradientReg(theta2, X2, y2, learningRate)
print(f"初始正则化成本: {initial_cost_reg:.12f}")
print(f"初始正则化梯度: {initial_grad_reg}")

# 2.4 寻找最优参数
print("[Step 2.4] 使用TNC优化正则化逻辑回归")
result2 = opt.fmin_tnc(func=costReg, x0=theta2, fprime=gradientReg, args=(X2, y2, learningRate))
theta2_opt = np.matrix(result2[0])
optimized_cost_reg = costReg(result2[0], X2, y2, learningRate)
print(f"优化后theta2: {result2[0]}")
print(f"优化后正则化成本: {optimized_cost_reg:.12f}")

# 2.5 评估
print("[Step 2.5] 评估正则化逻辑回归")
predictions2 = predict(theta2_opt, X2)
truth2 = y2.ravel().astype(int)
accuracy2 = float((np.array(predictions2) == truth2).mean() * 100)
print(f"regularized accuracy = {accuracy2:.2f}%")

# 正则化决策边界可视化
xx, yy = np.meshgrid(
    np.linspace(data2["Test 1"].min() - 0.1, data2["Test 1"].max() + 0.1, 300),
    np.linspace(data2["Test 2"].min() - 0.1, data2["Test 2"].max() + 0.1, 300),
)
grid = np.c_[xx.ravel(), yy.ravel()]
grid_mapped = map_feature(grid[:, 0], grid[:, 1], degree=degree)
zz = sigmoid(np.matrix(grid_mapped) * theta2_opt.T)
zz = np.asarray(zz).reshape(xx.shape)

fig, ax = plt.subplots(figsize=(12, 8))
ax.scatter(positive2["Test 1"], positive2["Test 2"], s=20, c="b", marker="o", label="Accepted")
ax.scatter(negative2["Test 1"], negative2["Test 2"], s=20, c="r", marker="x", label="Rejected")
ax.contour(xx, yy, zz, levels=[0.5], colors="g")
ax.legend()
ax.set_xlabel("Test 1 Score")
ax.set_ylabel("Test 2 Score")
ax.set_title("Step2.5 Regularized Decision Boundary")
fig.savefig(FIG_DIR / "step2_5_decision_boundary.png", dpi=180, bbox_inches="tight")
plt.close(fig)

summary = {
    "dataset_1": "sklearn breast_cancer (2 features)",
    "dataset_2": "sklearn digits PCA2D (binary)",
    "ex2data1_file": str(ex2data1_path),
    "ex2data2_file": str(ex2data2_path),
    "step1_initial_cost": initial_cost,
    "step1_initial_gradient": initial_grad.tolist(),
    "step1_theta_opt": np.asarray(theta_opt).ravel().tolist(),
    "step1_optimized_cost": optimized_cost,
    "step1_accuracy": accuracy,
    "step2_initial_cost_reg": initial_cost_reg,
    "step2_initial_gradient_reg": initial_grad_reg.tolist(),
    "step2_theta_opt": np.asarray(theta2_opt).ravel().tolist(),
    "step2_optimized_cost_reg": optimized_cost_reg,
    "step2_accuracy": accuracy2,
    "mapped_feature_count": int(data2_mapped.shape[1]),
    "figure_dir": str(FIG_DIR),
}

with open(OUTPUT_DIR / "summary.json", "w", encoding="utf-8") as f:
    json.dump(summary, f, ensure_ascii=False, indent=2)

print("\n全部步骤执行完成。")
print(f"图像输出目录: {FIG_DIR}")
print(f"结果摘要: {OUTPUT_DIR / 'summary.json'}")

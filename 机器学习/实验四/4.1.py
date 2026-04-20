# -*- coding: utf-8 -*-

import numpy as np
import matplotlib.pyplot as plt

plt.rcParams['font.sans-serif'] = ['SimHei']
plt.rcParams['axes.unicode_minus'] = False


def generate_toy_data(m=40, seed=0):

    # 随机生成二分类数据：

    rng = np.random.RandomState(seed)
    m_pos = m // 2
    m_neg = m - m_pos
    X_pos = rng.randn(m_pos, 2) + np.array([2.0, 2.0])
    X_neg = rng.randn(m_neg, 2) + np.array([-2.0, -2.0])
    X = np.vstack([X_pos, X_neg])
    y = np.hstack([np.ones(m_pos), -np.ones(m_neg)])
    return X, y


def load_manual_data():

    # 从键盘手动输入数据：

    print("请以 'x1 x2 y' 的形式输入训练样本，y 只能为 1 或 -1，输入 q 结束：")
    X_list = []
    y_list = []
    while True:
        s = input()
        if s.strip().lower() == "q":
            break
        parts = s.split()
        if len(parts) != 3:
            print("格式应为: x1 x2 y，例如: 1.0 2.0 1 ；输入 q 结束。")
            continue
        try:
            x1 = float(parts[0])
            x2 = float(parts[1])
            label = int(parts[2])
            if label not in (1, -1):
                print("标签 y 必须为 1 或 -1。")
                continue
        except ValueError:
            print("输入格式有误，请重新输入。")
            continue
        X_list.append([x1, x2])
        y_list.append(label)
    if len(X_list) == 0:
        print("未输入任何样本，改为使用随机数据。")
        return generate_toy_data()
    return np.array(X_list, dtype=float), np.array(y_list, dtype=float)


def svm_train_primal_hinge(X, y, C=1.0, lr=0.01, num_epochs=5000):

    # 使用梯度下降算法最小化损失

    m, n = X.shape
    w = np.zeros(n)
    b = 0.0

    for epoch in range(num_epochs):
        # 初始化梯度
        grad_w = w.copy()  # 来自 1/2 ||w||^2 的梯度
        grad_b = 0.0

        # 累加损失部分的梯度
        margins = y * (X @ w + b)  # 每个样本的 y_i (w^T x_i + b)
        mis_idx = margins < 1
        if np.any(mis_idx):
            grad_w -= C * np.sum((y[mis_idx][:, None] * X[mis_idx]), axis=0)
            grad_b -= C * np.sum(y[mis_idx])

        # 更新梯度
        w -= lr * grad_w
        b -= lr * grad_b


    return w, b


def plot_result(X, y, w, b, title="线性 SVM 分类结果"):

    plt.figure()
    X_pos = X[y == 1]
    X_neg = X[y == -1]
    plt.scatter(X_pos[:, 0], X_pos[:, 1], c='red', marker='o', label='y=1')
    plt.scatter(X_neg[:, 0], X_neg[:, 1], c='blue', marker='x', label='y=-1')

    # 画出决策边界和间隔线
    x1_min, x1_max = np.min(X[:, 0]) - 1, np.max(X[:, 0]) + 1
    xs = np.linspace(x1_min, x1_max, 200)
    # w1 * x1 + w2 * x2 + b = 0  =>  x2 = -(w1*x1 + b)/w2
    x2_decision = -(w[0] * xs + b) / w[1]
    x2_margin_up = -(w[0] * xs + b - 1) / w[1]
    x2_margin_dn = -(w[0] * xs + b + 1) / w[1]
    plt.plot(xs, x2_decision, 'g-', label='决策边界')
    plt.plot(xs, x2_margin_up, 'k--', linewidth=0.8, label='间隔边界')
    plt.plot(xs, x2_margin_dn, 'k--', linewidth=0.8)


    plt.xlabel("x1")
    plt.ylabel("x2")
    plt.title(title)
    plt.legend()
    plt.grid(True)
    plt.show()


def main():
    print("线性 SVM 基本型示例程序（二分类）")
    print("请输入 1 使用手动输入数据，或输入 2 使用随机生成的数据：")
    while True:
        c = input()
        if c == "1":
            X, y = load_manual_data()
            break
        elif c == "2":
            print("请输入数据规模：")
            try:
                m = int(input())
            except ValueError:
                m = 40
            X, y = generate_toy_data(m=m)
            break
        else:
            print("输入有误，请重新输入 1 或 2。")

    print("共得到 {} 个样本，开始训练 SVM 模型……".format(len(y)))
    w, b = svm_train_primal_hinge(X, y, C=1.0, lr=0.01, num_epochs=1000)
    print("训练结束，学得参数为：")
    print("w =", w)
    print("b =", b)

    # 计算训练集精度
    preds = np.sign(X @ w + b)
    acc = np.mean(preds == y)
    print("在训练集上的分类准确率为：{:.2f}%".format(acc * 100))

    # 绘制分类结果与分隔超平面
    plot_result(X, y, w, b, title="线性 SVM 基本型（梯度下降近似求解）")


if __name__ == "__main__":
    main()


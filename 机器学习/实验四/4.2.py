# -*- coding: utf-8 -*-

import numpy as np
import matplotlib.pyplot as plt
from cvxopt import matrix, solvers

plt.rcParams['font.sans-serif'] = ['SimHei']
plt.rcParams['axes.unicode_minus'] = False


def generate_toy_data(m=40, seed=0):

    # 随机生成二分类数据。

    rng = np.random.RandomState(seed)
    m_pos = m // 2
    m_neg = m - m_pos
    X_pos = rng.randn(m_pos, 2) + np.array([2.0, 2.0])
    X_neg = rng.randn(m_neg, 2) + np.array([-2.0, -2.0])
    X = np.vstack([X_pos, X_neg])
    y = np.hstack([np.ones(m_pos), -np.ones(m_neg)])
    return X, y


def load_manual_data():

    # 从键盘手动输入二维数据

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


def svm_solve_dual_cvxopt(X, y):

    # 使用 cvxopt 求解 SVM 的对偶问题。

    m, n = X.shape
    y = y.astype(float)

    # 计算核矩阵 K_ij = x_i^T x_j
    K = X @ X.T
    P = matrix(np.outer(y, y) * K)
    q = matrix(-np.ones(m))

    G = matrix(-np.eye(m))
    h = matrix(np.zeros(m))

    A = matrix(y.reshape(1, -1))
    b = matrix(np.zeros(1))

    # 关闭冗长输出
    solvers.options['show_progress'] = False
    solution = solvers.qp(P, q, G, h, A, b)

    alphas = np.array(solution['x']).reshape(-1)
    return alphas


def recover_w_b(X, y, alphas, tol=1e-5):

    # 根据 α 计算 w 和 b：

    w = np.sum((alphas * y)[:, None] * X, axis=0)
    # 选择一个支持向量
    sv_idx = np.where(alphas > tol)[0]
    if len(sv_idx) == 0:
        b = 0.0
    else:
        k = sv_idx[0]
        b = y[k] - np.dot(w, X[k])
    return w, b, sv_idx


def plot_result(X, y, w, b, sv_idx, title="cvxopt 求解的线性 SVM"):

    # 绘制训练样本点、支持向量和分隔超平面。

    plt.figure()
    X_pos = X[y == 1]
    X_neg = X[y == -1]
    plt.scatter(X_pos[:, 0], X_pos[:, 1], c='red', marker='o', label='y=1')
    plt.scatter(X_neg[:, 0], X_neg[:, 1], c='blue', marker='x', label='y=-1')

    # 标记支持向量
    if len(sv_idx) > 0:
        plt.scatter(X[sv_idx, 0], X[sv_idx, 1], s=120,
                    facecolors='none', edgecolors='k', linewidths=1.5,
                    label='支持向量')

    x1_min, x1_max = np.min(X[:, 0]) - 1, np.max(X[:, 0]) + 1
    xs = np.linspace(x1_min, x1_max, 200)
    if abs(w[1]) > 1e-6:
        x2_decision = -(w[0] * xs + b) / w[1]
        x2_margin_up = -(w[0] * xs + b - 1) / w[1]
        x2_margin_dn = -(w[0] * xs + b + 1) / w[1]
        plt.plot(xs, x2_decision, 'g-', label='决策边界')
        plt.plot(xs, x2_margin_up, 'k--', linewidth=0.8, label='间隔边界')
        plt.plot(xs, x2_margin_dn, 'k--', linewidth=0.8)
    else:
        print("w2 接近 0，分隔超平面近似竖直，暂不绘制。")

    plt.xlabel("x1")
    plt.ylabel("x2")
    plt.title(title)
    plt.legend()
    plt.grid(True)
    plt.show()


def main():
    print("使用 cvxopt 求解线性 SVM 对偶问题（二分类）")
    print("请输入 1 使用手动输入数据，或输入 2 使用随机生成的数据：")
    while True:
        c = input()
        if c == "1":
            X, y = load_manual_data()
            break
        elif c == "2":
            print("请输入数据规模2：")
            try:
                m = int(input())
            except ValueError:
                m = 40
            X, y = generate_toy_data(m=m)
            break
        else:
            print("输入有误，请重新输入 1 或 2。")

    print("共得到 {} 个样本，开始调用 cvxopt 求解对偶问题……".format(len(y)))
    alphas = svm_solve_dual_cvxopt(X, y)
    print("得到的 α（前 10 项）为：")
    print(alphas[:10])

    w, b, sv_idx = recover_w_b(X, y, alphas)
    print("恢复得到的参数：")
    print("w =", w)
    print("b =", b)
    print("支持向量个数 =", len(sv_idx))

    # 训练集精度
    preds = np.sign(X @ w + b)
    acc = np.mean(preds == y)
    print("在训练集上的分类准确率为：{:.2f}%".format(acc * 100))

    plot_result(X, y, w, b, sv_idx, title="cvxopt 求解线性 SVM（对偶）")


if __name__ == "__main__":
    main()


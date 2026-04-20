# -*- coding: utf-8 -*-

import numpy as np
import matplotlib.pyplot as plt

# 解决中文显示和负号问题
plt.rcParams['font.sans-serif'] = ['SimHei']
plt.rcParams['axes.unicode_minus'] = False


print("感知机模型：线性二分类程序")
print("请输入1，选择手动输入数据，或输入2，选择随机生成数据")

# 存放二维特征 x 与标签 y（取值为 +1 或 -1）
X_list = []
y_list = []

while True:
    c = input()
    if c == "1":
        print("请以 x1 x2 y 的形式输入样本，其中 y 只能为 1 或 -1，输入 -1 结束（仅输入一个 -1）")
        while True:
            s = input()
            if s.strip() == "-1":
                break
            parts = s.split()
            if len(parts) != 3:
                print("格式应为: x1 x2 y，例如: 1.0 2.0 1")
                continue
            x1 = float(parts[0])
            x2 = float(parts[1])
            y_val = int(parts[2])
            if y_val not in (1, -1):
                print("标签 y 必须为 1 或 -1")
                continue
            X_list.append([x1, x2])
            y_list.append(y_val)
        m = len(X_list)
        break
    elif c == "2":
        print("请输入随机生成的数据规模")
        m = int(input())
        # 随机生成二维特征
        X_list = np.random.randint(1, 100, size=(m, 2))
        # 构造线性超平面，使用 sign 决定标签，并生成模拟数据
        true_w = np.array([1.0, -0.5])
        true_b = -10.0
        scores = X_list @ true_w + true_b
        y_list = np.where(scores >= 0, 1, -1)
        break
    else:
        print("输入有误，请重新输入 1 或 2")

# 至少需要两个样本点
if m < 2:
    print("数据量不足")
else:
    # 将数据转换为 NumPy 数组，方便矩阵/向量运算
    X = np.array(X_list, dtype=float)   # 形状 (m, 2)
    y = np.array(y_list, dtype=int)     # 形状 (m,)

    # 感知机参数初始化
    w = np.zeros(2, dtype=float)
    b = 0.0

    # 设置学习率 η 和最大迭代轮数
    learning_rate = 1.0
    max_epochs = 1000

    print("开始训练感知机模型（最大迭代轮数：{}）".format(max_epochs))

    for epoch in range(max_epochs):
        error_count = 0
        #顺序扫描每一个样本
        for i in range(m):
            x_i = X[i]
            y_i = y[i]

            #输出 f(x) = w^T x + b
            fx = np.dot(w, x_i) + b

            #根据 sign 进行分类
            if y_i * fx <= 0:
                # 按照感知机学习规则更新 w 和 b
                w = w + learning_rate * y_i * x_i
                b = b + learning_rate * y_i
                error_count += 1

        print("第 {} 轮迭代，误分类样本数：{}".format(epoch + 1, error_count))

        # 如果本轮没有任何误分类，说明当前 (w,b) 已经能正确分类所有训练样本
        if error_count == 0:
            print("在第 {} 轮迭代时收敛，训练结束。".format(epoch + 1))
            break
    else:
        print("达到最大迭代次数仍有误分类样本，可能数据线性不可分。")

    print("学习到的参数为：w = {}, b = {:.4f}".format(w, b))

    # 绘制训练样本点和最终的分类直线
    plt.figure()

    # 用不同颜色标记正类和负类
    X_pos = X[y == 1]
    X_neg = X[y == -1]
    if len(X_pos) > 0:
        plt.scatter(X_pos[:, 0], X_pos[:, 1], color="red", label="y = 1")
    if len(X_neg) > 0:
        plt.scatter(X_neg[:, 0], X_neg[:, 1], color="blue", label="y = -1")

    # 得到直线方程：
    if abs(w[1]) > 1e-6:
        x1_min, x1_max = np.min(X[:, 0]) - 1, np.max(X[:, 0]) + 1
        x1_plot = np.linspace(x1_min, x1_max, 200)
        x2_plot = -(w[0] / w[1]) * x1_plot - b / w[1]
        plt.plot(x1_plot, x2_plot, color="green", label="感知机分类直线")
    else:
        print("w2 接近 0，分类直线接近垂直，暂不绘制直线。")

    plt.xlabel("x1")
    plt.ylabel("x2")
    plt.title("感知机模型：二维线性二分类结果")
    plt.legend()
    plt.grid(True)
    plt.show()


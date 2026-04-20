# -*- coding: utf-8 -*-

import numpy as np
import matplotlib.pyplot as plt

plt.rcParams['font.sans-serif'] = ['SimHei']
plt.rcParams['axes.unicode_minus'] = False

# Sigmoid 函数：将任意实数映射到 (0,1)，用于输出“概率”
def sigmoid(z):
    return 1.0 / (1.0 + np.exp(-z))


print("一维对率回归模型拟合程序")
print("请输入1，选择手动输入数据，或输入2，选择随机生成数据")

# 存放特征 x 与标签 y
lix = []
liy = []

while True:
    c = input()
    if c == "1":
        print("请以 x y 的形式输入所有样本，其中 y 只能为 0 或 1，输入 -1 代表结束")
        while True:
            s = input()
            if s == "-1":
                break
            order = s.split()
            # x 为实数特征，y 为 0/1 标签
            x_val = float(order[0])
            y_val = int(order[1])
            if y_val not in (0, 1):
                print("标签 y 必须为 0 或 1")
                continue
            lix.append(x_val)
            liy.append(y_val)
        m = len(lix)
        break
    elif c == "2":
        print("请输入数据规模")
        m = int(input())
        # 随机生成一维特征 x
        lix = np.random.randint(1, 100, size=m)
        # 使用“真实”线性函数通过 Sigmoid 生成概率，再按概率产生 0/1 标签
        true_w = 0.1
        true_b = -5
        probs = sigmoid(true_w * lix + true_b)
        liy = (np.random.rand(m) < probs).astype(int)
        break
    else:
        print("输入有误，请重新输入 1 或 2")

# 至少需要两个样本点进行拟合
if m < 2:
    print("数据量不足")
else:
    # 将数据转换为 NumPy 数组，方便进行向量化计算
    x = np.array(lix, dtype=float)
    y = np.array(liy, dtype=float)

    # 初始化参数 w, b；使用“最大似然法 + 梯度下降”来估计参数
    w = 0.0
    b = 0.0

    # 设置学习率（步长）和迭代次数：
    learning_rate = 0.001
    num_epochs = 10000

    for epoch in range(num_epochs):
        # 线性部分：z = w * x + b
        z = w * x + b

        # 使用 Sigmoid 函数，得到 p(y=1|x)
        p = sigmoid(z)

        # 按照课件“负对数似然”公式计算目标函数：
        loss = np.mean(-y * z + np.log(1 + np.exp(z)))

        # 求梯度：
        dw = np.mean((p - y) * x)
        db = np.mean(p - y)

        # 沿着“负梯度方向”更新参数，使 ℓ(β) 逐渐减小
        w -= learning_rate * dw
        b -= learning_rate * db

        # 输出当前的“平均负对数似然”
        if (epoch + 1) % 2000 == 0:
            print("第 {} 次迭代，当前平均负对数似然 ℓ(β)/m = {:.4f}".format(epoch + 1, loss))

    print("训练结束！")
    print("学习得到的参数为：w = {:.4f}, b = {:.4f}".format(w, b))

    # 根据训练好的参数，对训练集进行预测，计算分类准确率
    z_final = w * x + b
    p_final = sigmoid(z_final)
    y_pred = (p_final >= 0.5).astype(int)  # 概率 >=0.5 判为 1，否则判为 0
    accuracy = np.mean(y_pred == y)
    print("在训练数据上的分类准确率为：{:.2f}%".format(accuracy * 100))

    # 绘制数据点和 Sigmoid 拟合曲线
    plt.figure()
    x_pos = x[y == 1]
    x_neg = x[y == 0]
    plt.scatter(x_pos, y[y == 1], color="red", label="y=1 样本")
    plt.scatter(x_neg, y[y == 0], color="blue", label="y=0 样本")

    x_plot = np.linspace(np.min(x), np.max(x), 200)
    p_plot = sigmoid(w * x_plot + b)
    plt.plot(x_plot, p_plot, color="green", label="预测概率 Sigmoid 曲线")

    plt.xlabel("x")
    plt.ylabel("y / p(y=1|x)")
    plt.title("一维对率回归模型拟合结果")
    plt.legend()
    plt.grid(True)
    plt.show()


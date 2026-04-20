# -*- coding: utf-8 -*-
#引入库来生成随机数与绘制图像
import numpy as np
import matplotlib.pyplot as plt
lix = []
liy = []
tx = 0
sx = 0
d = 0
n = 0
w = 0
b = 0
MSE = 0
print("一元线性回归模型拟合程序")
print("请输入1，选择手动输入数据，或输入2， 选择随机生成数据")
while True:
    c = input()
    if c == "1":
        print("请以x y的形式,输入所有样本，输入-1代表结束")
        while True:
            s = input()
            if(s == "-1"):
                break
            order = s.split()
            lix.append(eval(order[0]))
            liy.append(eval(order[1]))
            m = len(lix)
        break
    elif c == "2":
        print("请输入数据规模")
        m = eval(input())
        lix = np.random.randint(1, 100, size = m)
        liy = np.random.randint(1, 100, size = m)
        break
    else:
        print("输入有误，请重新输入")
#为了绘制直线，要求至少有两组数据
if m < 2:
    print("数据量不足")
else:
    #计算x的平均值
    for t in range(m):
        tx = tx + lix[t]
    d = tx * tx / m
    tx = tx / m
    #根据公式进行计算w和b
    for t in range(m):
        n = n + (liy[t] * (lix[t] - tx))
        d = d - lix[t] ** 2
    d = d * -1
    w= n / d
    for t in range(m):
        b = b + (liy[t] - lix[t] * w)
    b = b / m
    #计算均方误差，评估拟合程度
    for t in range(m):
        MSE = (w * lix[t] + b - liy[t]) ** 2
    MSE = MSE / m
    print("计算得到的均方误差为" + str(MSE))
    #绘制数据点与直线
    plt.scatter(lix, liy, color='red', label='Data Points')
    plt.plot(lix, [w * x + b for x in lix], label = 'Fitted Line')
    plt.legend()
    plt.show()

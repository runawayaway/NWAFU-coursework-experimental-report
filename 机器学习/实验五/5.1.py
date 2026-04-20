# -*- coding: utf-8 -*-

import numpy as np
import csv
import math
import random

def load_watermelon_data(filename):

    # 从 CSV 文件中读取西瓜数据集3.0。
    X_disc_list = []
    X_cont_list = []
    y_list = []
    with open(filename, 'r', encoding='utf-8') as f:
        reader = csv.reader(f)
        # 跳过表头：色泽,根蒂,敲声,纹理,脐部,触感,密度,含糖率,好瓜
        header = next(reader)
        for row in reader:
            if not row:  # 跳过空行
                continue
            # 前 6 列为离散属性
            disc_attrs = row[0:6]
            # 第 6、7 列是密度和含糖率
            density = float(row[6])
            sugar = float(row[7])
            label_str = row[8].strip()
            if label_str == "是":
                label = 1
            else:
                label = 0
            X_disc_list.append(disc_attrs)
            X_cont_list.append([density, sugar])
            y_list.append(label)
    X_disc = np.array(X_disc_list, dtype=object)
    X_cont = np.array(X_cont_list, dtype=float)
    y = np.array(y_list, dtype=int)
    return X_disc, X_cont, y


def estimate_gaussian_nb_params(X_cont, y):

    # 计算先验概率 p(C=1)、p(C=0)与各类别下，每个特征的均值 μ 和标准差 σ
    classes = [0, 1]
    priors = {}
    means = {}
    stds = {}

    m = len(y)
    for c in classes:
        # 取出属于类别 c 的样本
        X_c = X_cont[y == c]
        # 先验概率：p(C=c) = 该类样本数 / 总样本数
        priors[c] = len(X_c) / float(m)
        # 均值 μ_cj、标准差 σ_cj：最大似然估计
        mu = X_c.mean(axis=0)
        sigma = X_c.std(axis=0, ddof=1)
        epsilon = 1e-6
        sigma[sigma < epsilon] = epsilon
        means[c] = mu
        stds[c] = sigma
    return priors, means, stds


def estimate_discrete_nb_params(X_disc, y):

    # 加入拉普拉斯修正：
    m, d_disc = X_disc.shape
    classes = [0, 1]

    # 统计每个属性可能取值
    attr_values = []
    for i in range(d_disc):
        values = sorted(list(set(X_disc[:, i].tolist())))
        attr_values.append(values)

    # 条件概率表
    cond_prob = {c: [{} for _ in range(d_disc)] for c in classes}

    for c in classes:
        X_c = X_disc[y == c]
        N_c = len(X_c)
        for i in range(d_disc):
            values = attr_values[i]
            N_i = len(values)
            for v in values:
                # 统计在类别 c 下，第 i 个属性取值为 v 的样本数
                count_cv = np.sum(X_c[:, i] == v)
                prob = (count_cv + 1.0) / (N_c + N_i)  # 拉普拉斯修正
                cond_prob[c][i][v] = prob

    return cond_prob, attr_values


def gaussian_pdf(x, mu, sigma):

    # 计算 p(x_j | C)。
    coeff = 1.0 / (math.sqrt(2.0 * math.pi) * sigma)
    exponent = - (x - mu) ** 2 / (2.0 * (sigma ** 2))
    return coeff * math.exp(exponent)


def predict_proba(x_disc, x_cont, priors, means, stds, cond_prob, attr_values):
    scores = {}
    for c in [0, 1]:
        # 计算对数似然
        log_prob = math.log(priors[c] + 1e-12)
        for i in range(len(x_disc)):
            v = x_disc[i]
            values = attr_values[i]
            N_i = len(values)
            if v in cond_prob[c][i]:
                p_xi_c = cond_prob[c][i][v]
            else:
                # count=0 的情形下需要 y，但在函数内不可见，简化为极小值
                N_c = np.sum(y == c)
                p_xi_c = 1.0 / (1e6)
            log_prob += math.log(p_xi_c + 1e-12)

        # 使用高斯密度
        for j in range(len(x_cont)):
            mu = means[c][j]
            sigma = stds[c][j]
            log_prob += -0.5 * math.log(2.0 * math.pi) - math.log(sigma) \
                        - ((x_cont[j] - mu) ** 2) / (2.0 * (sigma ** 2))
        scores[c] = math.exp(log_prob)

    # 归一化得到后验概率
    total = sum(scores.values())
    for c in scores:
        scores[c] = scores[c] / total
    return scores


def main():
    print("朴素贝叶斯分类器：根据西瓜全部属性判断是否为好瓜")
    print("数据来源：'西瓜数据集3.0.csv'")

    # 读取并展示数据规模
    X_disc, X_cont, y = load_watermelon_data("西瓜数据集3.0.csv")
    m = len(y)
    print("共读取到 {} 个样本，其中好瓜 {} 个，坏瓜 {} 个。".format(
        m, int((y == 1).sum()), int((y == 0).sum())
    ))

    # 估计先验概率和高斯分布参数
    priors, means, stds = estimate_gaussian_nb_params(X_cont, y)
    # 估计离散属性的条件概率
    cond_prob, attr_values = estimate_discrete_nb_params(X_disc, y)

    print("\n根据训练数据得到的先验概率和高斯分布参数（连续属性）：")
    for c in [0, 1]:
        print("类别 {}: 先验 p(C={}) = {:.3f}, 密度均值={:.3f}, 方差={:.5f}, 含糖率均值={:.3f}, 方差={:.5f}".format(
            "好瓜" if c == 1 else "坏瓜",
            c,
            priors[c],
            means[c][0], stds[c][0] ** 2,
            means[c][1], stds[c][1] ** 2
        ))

    print("\n现在进行分类预测：")
    print("请输入1，手动输入一个西瓜的全部属性；")
    print("或输入2，随机从数据集中抽取一个西瓜进行预测")

    while True:
        choice = input("请输入选项(1/2)，其他键退出：")
        if choice not in ["1", "2"]:
            print("程序结束。")
            break

        if choice == "1":
            try:
                color = input("请输入色泽（例如 青绿 / 乌黑 / 浅白）：")
                root = input("请输入根蒂（例如 蜷缩 / 稍蜷 / 硬挺）：")
                sound = input("请输入敲声（例如 浊响 / 沉闷 / 清脆）：")
                texture = input("请输入纹理（例如 清晰 / 稍糊 / 模糊）：")
                navel = input("请输入脐部（例如 凹陷 / 稍凹 / 平坦）：")
                touch = input("请输入触感（例如 硬滑 / 软粘）：")
                dens_str = input("请输入该西瓜的密度（例如 0.697）：")
                sug_str = input("请输入该西瓜的含糖率（例如 0.460）：")
                dens = float(dens_str)
                sug = float(sug_str)
                x_disc_new = [color, root, sound, texture, navel, touch]
                x_cont_new = [dens, sug]
                print("您输入的样本特征：{}, {}, {}, {}, {}, {}, 密度={:.3f}, 含糖率={:.3f}".format(
                    color, root, sound, texture, navel, touch, dens, sug
                ))
                from_dataset = False
            except ValueError:
                print("输入格式有误，请重新输入。")
                continue
        else:
            # 从数据集中随机抽取一个样本作为测试数据
            idx = random.randint(0, m - 1)
            x_disc_new = X_disc[idx, :].tolist()
            dens = X_cont[idx, 0]
            sug = X_cont[idx, 1]
            x_cont_new = [dens, sug]
            true_label = y[idx]
            print("随机选取的样本索引为 {}:".format(idx))
            print("  色泽={}, 根蒂={}, 敲声={}, 纹理={}, 脐部={}, 触感={}, 密度={:.3f}, 含糖率={:.3f}, 实际标签={}".format(
                x_disc_new[0], x_disc_new[1], x_disc_new[2],
                x_disc_new[3], x_disc_new[4], x_disc_new[5],
                dens, sug,
                "好瓜" if true_label == 1 else "坏瓜"
            ))
            from_dataset = True

        # 计算后验概率并给出分类结果
        post = predict_proba(x_disc_new, x_cont_new, priors, means, stds, cond_prob, attr_values)
        prob_good = post[1]
        prob_bad = post[0]
        pred_label = 1 if prob_good >= prob_bad else 0

        print("根据朴素贝叶斯模型计算得到：")
        print("P(好瓜 | x) = {:.4f}, P(坏瓜 | x) = {:.4f}".format(prob_good, prob_bad))
        print("判定结果：该西瓜是 -> {}".format("好瓜" if pred_label == 1 else "坏瓜"))

        if from_dataset:
            if pred_label == true_label:
                print("预测正确！")
            else:
                print("预测错误。")

if __name__ == "__main__":
    main()


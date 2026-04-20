
clear; clc; close all;

% 参数定义
d = 24;          % 输入层节点数
q = 25;          % 隐含层节点数
l = 4;           % 输出层节点数


% 训练参数设置
eta = 0.1;       % 学习率 η 
max_epochs = 1000;  % 最大迭代次数
tol = 1e-6;      % 收敛容差

% 参数初始化
% 权重矩阵初始化
% v_ih: 输入层到隐含层的权重矩阵，大小为 d×q
v = randn(d, q) * 0.1;  % 输入层到隐含层权重

% w_hj: 隐含层到输出层的权重矩阵，大小为 q×l
w = randn(q, l) * 0.1;  % 隐含层到输出层权重

% 偏置阈值初始化
% γ_h: 隐含层神经元的阈值，大小为 q×1
gamma = randn(q, 1) * 0.1;  % 隐含层偏置

% θ_j: 输出层神经元的阈值，大小为 l×1
theta = randn(l, 1) * 0.1;  % 输出层偏置


% Sigmoid激活函数及其导数
sigmoid = @(x) 1 ./ (1 + exp(-x));
sigmoid_derivative = @(x) x .* (1 - x); 


fprintf('BP神经网络分类器程序\n');
fprintf('请输入1，选择手动输入数据，或输入2，选择随机生成数据\n');
choice = input('');

if choice == 1
    % 手动输入模式
    fprintf('请以样本形式输入：先输入特征向量（24维），再输入标签向量（4维），输入-1结束\n');
    X = [];  
    Y = [];  
    
    while true
        fprintf('输入24个特征值（用空格分隔）:\n');
        feature_str = input('', 's');
        if strcmp(feature_str, '-1')
            break;
        end
        features = str2num(feature_str);
        if length(features) ~= d
            fprintf('特征维度错误，应为%d维\n', d);
            continue;
        end
        
        fprintf('输入4个标签值（用空格分隔，例如：1 0 0 0）:\n');
        label_str = input('', 's');
        labels = str2num(label_str);
        if length(labels) ~= l
            fprintf('标签维度错误，应为%d维\n', l);
            continue;
        end
        
        X = [X; features];
        Y = [Y; labels];
    end
    
    m = size(X, 1);  % 样本数量
    
elseif choice == 2
    % 随机生成模式
    fprintf('请输入数据规模（样本数量）:\n');
    m = input('');
    % 随机生成输入特征（24维，归一化到[0,1]）
    X = rand(m, d);
    % 随机生成标签（4维，每个样本属于4个类别之一）
    Y = zeros(m, l);
    for i = 1:m
        class_idx = randi(l);
        Y(i, class_idx) = 1;
    end
    
else
    error('输入有误，请重新运行程序');
end

if m < 1
    error('数据量不足');
end

fprintf('数据准备完成，样本数量：%d\n', m);


% BP算法训练过程
fprintf('开始训练...\n');
epoch = 0;
prev_error = inf;

while epoch < max_epochs
    epoch = epoch + 1;
    total_error = 0;
    for k = 1:m
        % 前向传播过程,获取第k个样本的输入和真实输出
        x_k = X(k, :)';      % 输入向量，大小为 d×1
        y_k = Y(k, :)';      % 真实标签向量，大小为 l×1
        
        % 计算隐含层神经元的输入和输出
        alpha = zeros(q, 1); 
        for h = 1:q
            alpha_h = 0;
            for i = 1:d
                % α_h = Σ_{i=1}^d v_{ih} * x_i
                alpha_h = alpha_h + v(i, h) * x_k(i);
            end
            alpha(h) = alpha_h;
        end
        
        % 隐含层神经元的输出：b_h = f(α_h - γ_h)
        b = zeros(q, 1); 
        for h = 1:q
            % b_h = f(α_h - γ_h)
            b(h) = sigmoid(alpha(h) - gamma(h));
        end
        
        % 计算输出层神经元的输入和输出
        % 第j个输出神经元的输入
        beta = zeros(l, 1); 
        for j = 1:l
            beta_j = 0;
            for h = 1:q
                % β_j = Σ_{h=1}^q w_{hj} * b_h
                beta_j = beta_j + w(h, j) * b(h);
            end
            beta(j) = beta_j;
        end
        
        % 输出层神经元的预测输出
        y_hat_k = zeros(l, 1); 
        for j = 1:l
            % ŷ_j^k = f(β_j - θ_j)
            y_hat_k(j) = sigmoid(beta(j) - theta(j));
        end
        
        %  计算均方误差
        E_k = 0;
        for j = 1:l
            E_k = E_k + (y_hat_k(j) - y_k(j))^2;
        end
        E_k = 0.5 * E_k;
        total_error = total_error + E_k;
        
        % 反向传播过程        
        % 计算输出层误差项
        g = zeros(l, 1);  
        for j = 1:l
            % g_j = ŷ_j^k(1-ŷ_j^k)(y_j^k - ŷ_j^k)
            g(j) = y_hat_k(j) * (1 - y_hat_k(j)) * (y_k(j) - y_hat_k(j));
        end
        
        % 更新隐含层到输出层的权重
        for j = 1:l
            for h = 1:q
                % Δw_hj = η * g_j * b_h
                delta_w = eta * g(j) * b(h);
                w(h, j) = w(h, j) + delta_w;
            end
        end
        
        % 更新输出层偏置
        for j = 1:l
            % Δθ_j = -η * g_j
            delta_theta = -eta * g(j);
            theta(j) = theta(j) + delta_theta;
        end
        
        % 计算隐含层误差项
        % e_h = b_h(1 - b_h) * Σ_{j=1}^l w_hj * g_j
        e = zeros(q, 1);
        for h = 1:q
            sum_term = 0;
            for j = 1:l
                % Σ_{j=1}^l w_hj * g_j
                sum_term = sum_term + w(h, j) * g(j);
            end
            % e_h = b_h(1 - b_h) * Σ_{j=1}^l w_hj * g_j
            e(h) = b(h) * (1 - b(h)) * sum_term;
        end
        
        % 更新输入层到隐含层的权重
        for h = 1:q
            for i = 1:d
                % Δv_ih = η * e_h * x_i
                delta_v = eta * e(h) * x_k(i);
                v(i, h) = v(i, h) + delta_v;
            end
        end
        
        % 更新隐含层偏置
        for h = 1:q
            % Δγ_h = -η * e_h
            delta_gamma = -eta * e(h);
            gamma(h) = gamma(h) + delta_gamma;
        end
        
    end
    
    % 计算平均误差
    avg_error = total_error / m;
    
    % 输出训练进度
    if mod(epoch, 100) == 0 || epoch == 1
        fprintf('迭代次数：%d，平均误差：%.6f\n', epoch, avg_error);
    end
    
    prev_error = avg_error;
    
end

if epoch >= max_epochs
    fprintf('达到最大迭代次数：%d，最终误差：%.6f\n', max_epochs, prev_error);
end

% 测试阶段
fprintf('\n开始测试...\n');
correct = 0;

for k = 1:m
    x_k = X(k, :)';
    y_k = Y(k, :)';
    
    % 前向传播计算预测输出
    % 隐含层输入和输出
    alpha = zeros(q, 1);
    for h = 1:q
        alpha_h = 0;
        for i = 1:d
            alpha_h = alpha_h + v(i, h) * x_k(i);
        end
        alpha(h) = alpha_h;
    end
    
    b = zeros(q, 1);
    for h = 1:q
        b(h) = sigmoid(alpha(h) - gamma(h));
    end
    
    % 输出层输入和输出
    beta = zeros(l, 1);
    for j = 1:l
        beta_j = 0;
        for h = 1:q
            beta_j = beta_j + w(h, j) * b(h);
        end
        beta(j) = beta_j;
    end
    
    y_hat_k = zeros(l, 1);
    for j = 1:l
        y_hat_k(j) = sigmoid(beta(j) - theta(j));
    end
    
    % 找到预测类别和真实类别
    [~, pred_class] = max(y_hat_k);
    [~, true_class] = max(y_k);
    
    if pred_class == true_class
        correct = correct + 1;
    end
end

accuracy = correct / m * 100;
fprintf('训练集准确率：%.2f%% (%d/%d)\n', accuracy, correct, m);

#include <bits/stdc++.h>
#include <ctime>
#include <time.h>
#include <stdlib.h>
#include <windows.h>
#include "DFS.h"
#include "DFS(Optimization).h"
#include "DP.h"
using namespace std;
int arr[105][105], path[105][105];
clock_t st, et;
double dt;
//生成指定规模随机矩阵用于测试
void random(int n, int maxn)
{
    for(int i = 1; i <= n; i++)
    {
        for(int j = 1; j <= n; j++)
        {
            arr[i][j] = rand() % maxn;
        }
    }
}
//设置输出的颜色，用于区分两条路径
void setColor(int color)
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(handle, color);
}
void Print(int n)
{
    for(int i = 1; i <= n; i++)
    {
        for(int j = 1; j <= n; j++)
        {
			//0表示未被经过，用白色表示
            if(path[i][j] == 0)
            {
                setColor(FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
                printf("%5d", arr[i][j]);
            }
			//1表示被第一条路径经过，用红色表示
            else if(path[i][j] == 1)
            {
                setColor(FOREGROUND_INTENSITY | FOREGROUND_RED);
                printf("%5d", arr[i][j]);
            }
			//2表示被第二条路径经过，用黄色表示
            else if(path[i][j] == 2)
            {
                setColor(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
                printf("%5d", arr[i][j]);
            }
			//3表示被两条路径同时经过，用蓝色表示
            else if(path[i][j] == 3)
            {
                setColor(FOREGROUND_INTENSITY | FOREGROUND_BLUE);
                printf("%5d", arr[i][j]);
            }
        }
        printf("\n");
    }
    setColor(FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
}
void init(int mode)
{
    int n, maxn, ans;
	//输入方格大小与样本价值最大值
    while(true)
    {
        printf("请输入方阵大小（1-100）：");
        scanf("%d", &n);
        if(n >= 1 && n <= 100) break;
        else printf("输入数据范围有误，请重新输入\n");
    }
    while(true)
    {
        printf("请输入样本价值最大值（1-1000）：");
        scanf("%d", &maxn);
        if(maxn > 0 ) break;
        else printf("输入数据范围有误，请重新输入\n");
    }
    srand(time(0));
    random(n, maxn);
	//根据选择的模型进行输出
    if(mode == 1 || mode == 4)
    {
       	//初始化路径
memset(path, 0, sizeof(path));
		//计算需要的时间
        st = clock();
		//调用对应的算法
        ans = dfs(n, arr, path);
        et = clock();
        dt = (double)(et - st) / 1000;
        printf("DFS计算得到的样本总价值为%d\n", ans);
        printf("DFS所花费的时间为%.3f秒\n", dt);
        path[1][1] = path[n][n] = 3;
        Print(n);
    }
    if(mode == 2 || mode == 4)
    {
        memset(path, 0, sizeof(path));
        st = clock();
        ans = dfsOp(n, arr, path);
        et = clock();
        dt = (double)(et - st) / 1000;
        printf("DFS（优化）计算得到的样本总价值为%d\n", ans);
        printf("DFS（优化）所花费的时间为%.3f秒\n", dt);
        path[1][1] = path[n][n] = 3;
        Print(n);
    }
    if(mode == 3 || mode == 4)
    {
        memset(path, 0, sizeof(path));
        st = clock();
        ans = dp(n, arr, path);
        et = clock();
        dt = (double)(et - st) / 1000;
        printf("DP计算得到的样本总价值为%d\n", ans);
        printf("DP所花费的时间为%.3f秒\n", dt);
        path[1][1] = path[n][n] = 3;
        Print(n);
    }
}
int main()
{
    while(true)
    {
        //设置简单的界面
printf("\t\t\t\t收集样本问题解决程序\n");
        printf("\n\t\t\t1.DFS");
        printf("\n\t\t\t2.DFS（优化）");
        printf("\n\t\t\t3.DP");
        printf("\n\t\t\t4.三种算法运行时间对比");
        printf("\n\t\t\t0.退出程序");
        printf("\n\n\t\t\t\t选择所要操作:");
        int m;
        scanf("%d", &m);
		//根据选择进入对应的模式
        switch(m)
        {
            case 1:
                init(1);
                break;
            case 2:
                init(2);
                break;
            case 3:
                init(3);
                break;
            case 4:
                init(4);
                break;
            case 0:
                return 0;
            default:
                printf("选择错误,重新选择.\n");
        }
    }
}

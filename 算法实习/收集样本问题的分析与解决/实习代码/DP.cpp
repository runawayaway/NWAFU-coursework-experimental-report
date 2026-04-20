#include<bits/stdc++.h>
using namespace std;
//dps用于存储dp过程的信息，mp用于记录最近路径方向
static int dps[105][105][105];
static int mp[105][105][105];
static void FindPath(int n, int x1,int y1, int x2, int path[][105])
{
    while(!(x1 == 1 && y1 == 1 && x2 == 1 ))
    {
        // 标记当前路径点
        path[x1][y1] += 1;
        path[x2][x1 + y1 - x2] += 2;
        // 根据存储的方向信息回溯
        int fwd = mp[x1][y1][x2];
        //都向左边移动
        if(fwd == 1) y1--;
        //都向上方移动
        else if(fwd == 2) x1--, x2--;
        //路径1向左移动，路径2向上移动
        else if(fwd == 3) y1--, x2--;
        //路径1向上移动，路径2向左移动
        else if(fwd == 4) x1--;
    }

}
int dp(int n, int arr[][105], int path[][105])
{
    for(int x1 = 1; x1 <= n; x1++)
    {
		for(int y1 = 1; y1 <= n; y1++)
		{
			for(int x2 = 1; x2 <= min(x1 + y1 - 1, n); x2++)
			{
				int maxn = -1;
				int fwd = 0;
				// 当两条路径在同一个点时
				if(x1 == x2)
				{
                    //路径1从左边来，路径2从上方来
                    if(dps[x1][y1 - 1][x2 - 1] > maxn)
                    {
                        fwd = 3;
                        maxn = dps[x1][y1 - 1][x2 - 1];
                    }
                    //路径1从上方来，路径2从右边来
                    if(dps[x1 - 1][y1][x2] > maxn)
                    {
                        fwd = 4;
                        maxn = dps[x1 - 1][y1][x2];
                    }
                    dps[x1][y1][x2] = arr[x1][y1] + maxn;
				}
				 // 当两条路径在不同点时
				else
                {
                    //都从左边来
                    if(dps[x1][y1 - 1][x2] > maxn)
                    {
                        fwd = 1;
                        maxn = dps[x1][y1 - 1][x2];
                    }
                    // 都从上方来
                    if(dps[x1 - 1][y1][x2 - 1] > maxn)
                    {
                        fwd = 2;
                        maxn = dps[x1 - 1][y1][x2 - 1];
                    }
                    //路径1从左边来，路径2从上方来
                    if(dps[x1][y1 - 1][x2 - 1] > maxn)
                    {
                        fwd = 3;
                        maxn = dps[x1][y1 - 1][x2 - 1];
                    }
                    //路径1从上方来，路径2从左边来
                    if(dps[x1 - 1][y1][x2] > maxn)
                    {
                        fwd = 4;
                        maxn = dps[x1 - 1][y1][x2];
                    }
                    // 计算当前增加的值
                    dps[x1][y1][x2] = arr[x1][y1] + arr[x2][x1 + y1 - x2] + maxn;
                }
                // 记录最优路径方向
                mp[x1][y1][x2] = fwd;
			}
		}
	}
	// 重建路径，由于是转换思路且是递推，所以重构的起点改为路径的终点
	FindPath(n, n, n, n, path);
	return dps[n][n][n];
}

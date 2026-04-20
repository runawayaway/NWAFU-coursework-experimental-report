#include<bits/stdc++.h>
using namespace std;
static int ans;
static int flag[105][105];
//x，y表示当前左边，stage记录当前是从起点到终点还是从终点到起点，arr存储每个方格中的数字,path用于记录最佳路径，flag记录当前路径
void Find(int x, int y, int tot, int stage, int n, int arr[][105], int path[][105])
{

	//边界检查
	if(x < 1 || y < 1 ||  x > n || y > n) return;
	//到达终点时切换阶段
	if(x == n && y == n) stage = 1;
	// 到达起点且是处于返回路径时完成时更新答案
	if(x == 1 && y == 1 && stage == 1)
    {
         //如果当前的总和大于目前记录的最大值，进行更新
        if(tot > ans)
        {
            ans = max(ans, tot);
            // 将flag中的路径复制到path中
            for(int i = 1; i <= n; i++)
            {
                for(int j = 1; j <= n; j++)
                {
                    path[i][j] = flag[i][j];
                }
            }
        }
    }
    //从起点到终点的过程
	if(!stage)
	{
         // 向下移动并进行搜索
        int t;
        t = arr[x + 1][y];
        arr[x + 1][y] = 0;
        flag[x + 1][y] += 1;
        Find(x + 1, y, tot + t, stage, n, arr, path);
        arr[x + 1][y] = t;
        flag[x + 1][y] -= 1;

         //向右移动并进行搜索
        t = arr[x][y + 1];
        arr[x][y + 1] = 0;
        flag[x][y + 1] += 1;
        Find(x, y + 1, tot + t, stage, n, arr, path);
        arr[x][y + 1] = t;
        flag[x][y + 1] -= 1;

	}
    //从终点回起点的过程
	else
	{
        int t;
        //向上移动并进行搜索
        t = arr[x - 1][y];
        arr[x - 1][y] = 0;
        flag[x - 1][y] += 2;
        Find(x - 1, y, tot + t, stage, n, arr, path);
        arr[x - 1][y] = t;
        flag[x - 1][y] -= 2;

        // 向左移动并进行搜索
        t = arr[x][y - 1];
        arr[x][y - 1] = 0;
        flag[x][y - 1] += 2;
        Find(x, y - 1, tot + t, stage, n, arr, path);
        arr[x][y - 1] = t;
        flag[x][y - 1] -= 2;
	}
}
int dfs(int n, int arr[][105], int path[][105])
{
    //初始化
    ans = 0;
    memset(flag, 0, sizeof(flag));
    Find(1, 1, 0, 0, n, arr, path);
    return ans;
}


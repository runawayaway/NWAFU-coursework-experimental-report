#include<bits/stdc++.h>
using namespace std;
static int ans;
static int flag[105][105];
static int mark[105][105][105][105];
static map<pair<pair<int, int>,pair<int, int>>, int>mp;
static int Find(int x1, int y1, int x2, int y2, int n, int arr[][105], int path[][105])
{
    int fwd = 0;
    //检查目标方格是否被搜索过
    if(mark[x1][y1][x2][y2] != -1) return mark[x1][y1][x2][y2];
    // 如果两条路径都到达终点，终止
    if(x1 == n && y1 == n && x2 == n && y2 == n) return 0;
    int tot = -1;
    //两条路径都向下移动
    if(x1 < n && x2 < n)
    {
        int tmp = Find(x1 + 1, y1, x2 + 1, y2, n, arr, path) + arr[x1 + 1][y1] + arr[x2 + 1][y2];
        //避免计算重复路径
        if(x1 == x2  && y1 == y2) tmp -= arr[x1 + 1][y1];
        if(tmp >= tot)
        {
            tot = tmp;
            fwd = 1;
        }
    }
    //两条路径都向右移动
    if(y1 < n && y2 < n)
    {
        int tmp = Find(x1, y1 + 1, x2, y2 + 1, n, arr, path) + arr[x1][y1 + 1] + arr[x2][y2 + 1];
        if(x1 == x2  && y1 == y2) tmp -= arr[x1][y1 + 1];
        if(tmp >= tot)
        {
            tot = tmp;
            fwd = 2;
        }
    }
    //路径1向下，路径2向右
    if(x1 < n && y2 < n)
    {
        int tmp = Find(x1 + 1, y1, x2, y2 + 1, n, arr, path) + arr[x1 + 1][y1] + arr[x2][y2 + 1];
        if(x1 + 1 == x2  && y1 == y2 + 1) tmp -= arr[x1 + 1][y1];
        if(tmp >= tot)
        {
            tot = tmp;
            fwd = 3;
        }
    }
    //路径1向右，路径2向下
    if(y1 < n && x2 < n)
    {
        int tmp = Find(x1, y1 + 1, x2 + 1, y2, n, arr, path) + arr[x1][y1 + 1] + arr[x2 + 1][y2];
        if(x1 == x2 + 1 && y1 + 1 == y2) tmp -= arr[x1][y1 + 1];
        if(tmp >= tot)
        {
            tot = tmp;
            fwd = 4;
        }
    }
    // 存储最优解和路径方向，为减少内存空间的消耗，使用map来存储路径方向
    mp[{make_pair(x1, y1), make_pair(x2, y2)}] = fwd;
    mark[x1][y1][x2][y2] = tot;
    //返回最大值
    return tot;
}
static void FindPath(int n, int x1,int y1, int x2, int y2, int path[][105])
{
    while(!(x1 == n && y1 == n && x2 == n && y2 == n))
    {
        //1表示路径1经过,2表示路径2经过,3表示两条路径都经过
        path[x1][y1] += 1;
        path[x2][y2] += 2;
        //根据存储的方向信息移动
        int fwd = mp[{make_pair(x1, y1), make_pair(x2, y2)}];
        //都向下
        if(fwd == 1) x1++, x2++;
        //都向右
        else if(fwd == 2) y1++, y2++;
        //路径1向下，路径2向右
        else if(fwd == 3) x1++, y2++;
        //路径1向右，路径2向左
        else if(fwd == 4) y1++, x2++;
    }

}
int dfsOp(int n, int arr[][105], int path[][105])
{
    //初始化
    ans = 0;
    memset(flag, 0, sizeof(flag));
    memset(mark, -1, sizeof(mark));
    ans = Find(1, 1, 1, 1, n, arr, path);
    ans += arr[1][1];
    //由于从起点开始搜索，所以此时应该从起点开始重建道路
    FindPath(n, 1, 1, 1, 1, path);
    return ans;
}


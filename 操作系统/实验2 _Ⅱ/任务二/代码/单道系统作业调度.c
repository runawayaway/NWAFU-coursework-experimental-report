#define _CRT_SECURE_NO_WARNINGS 1
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<conio.h>
#define getpch(type) (type*)malloc(sizeof(type))
int n;
float T1=0,T2=0;
int times=0;

struct jcb      //作业控制块
{
    char name[10];  //作业的名称标识
    int reachtime;   //作业的到达时间
    int starttime;    //作业的开始被处理的时间
    int needtime;       //作业的需求的服务时间
    int finishtime;       //作业完成时间
    float cycletime;       //作业周转时间
    float cltime;           //作业的带权周转时间
    char state;            //作业状态
    struct jcb *next;      //结构体指针
}*ready=NULL,*save, *p,*q, *c;

typedef struct jcb JCB;
void Copy()
{
    T1 = 0;
    T2 = 0;
    times = 0;
    p = save;
    ready = NULL;
    JCB *tail = NULL;
    while (p != NULL) {
        c = getpch(JCB);
        *c = *p;  // 复制内容
        c->next = NULL;
        if (ready == NULL) {
            ready = tail = c;
        } else {
            tail->next = c;
            tail = c;
        }
        p = p->next;
    }
}
int inital()   //建立作业控制块队列,先将其排成先来先服务的模式队列
{
    int i;
    printf("\n输入作业数:");
    scanf("%d", &n);
    for (i = 0; i<n; i++)
    {
        p = getpch(JCB);
        printf("\n输入作业名:");
        scanf("%s", p->name);
        getchar();
        p->reachtime = i;
        printf("作业默认到达时间:%d", i);
        printf("\n输入作业要运行的时间:");
        scanf("%d", &p->needtime);
        p->state = 'W';
        p->next = NULL;
        if (save == NULL)
        save = q = p;
        else
        {
                q->next = p;
                q = p;
        }
    }
    return n;
}

void disp(JCB*q,int m) //显示作业运行后的周转时间及带权周转时间等
{
    printf("\n作业%s正在运行，估计其运行情况：\n",q->name);
    printf("开始运行时刻：%d\n",q->starttime);
    printf("完成时刻：%d\n",q->finishtime);
    printf("周转时间：%f\n",q->cycletime);
    printf("带权周转时间：%f\n",q->cltime);
    getchar();
}

void running(JCB *p, int m)  //运行作业
{
    if (p == ready)     //先将要运行的作业从队列中分离出来
    {
    ready = p->next;
    p->next = NULL;
    }
    else
    {
        q = ready;
        while (q->next != p)  q = q->next;
        q->next = p->next;
    }
    p->starttime = times;    //计算作业运行后的完成时间,周转时间等等
    p->state = 'R';
    p->finishtime = p->starttime + p->needtime;
    p->cycletime = (float)(p->finishtime - p->reachtime);
    p->cltime = (float)(p->cycletime / p->needtime);
    T1 += p->cycletime;
    T2 += p->cltime;
    disp(p, m);        //调用disp()函数,显示作业运行情况
    times += p->needtime;
    p->state = 'F';
    printf("\n%s has been finished!\n", p->name);
    free(p);          //释放运行后的作业
    getchar();
}

void final(char* name) //最后打印作业的平均周转时间,平均带权周转时间
{
    float s,t;
    t=T1/n;
    s=T2/n;
    getchar();
    printf("%s\n", name);
    printf("\n作业已经全部完成!\n");
    printf("\n%d个作业的平均周转时间是：%f",n,t);
    printf("\n%d个作业的平均带权周转时间是%f：\n\n\n",n,s);
}

void sjf(int m)      // 最短作业优先算法
{
    JCB *min;
    int i,iden;
    Copy();
    for(i=0;i<n;i++)
    {
        p=min=ready;
        iden=1;
        do{
                if(p->state=='W'&&p->reachtime<=times)
                if(iden)
                {
                    min=p;
                    iden=0;
                }
                else if(p->needtime<min->needtime) min=p;
                p = p -> next; // 1 根据上下文，空格处填写一条指令
            } while (p != NULL);
        if(iden)
        {
            i--;
            times++;
            if (times>100)
            {
             printf("\nruntime is too long … error");
             getchar();
            }
        }
        else
        {
            running(min, m);
        }
    }
    final((char*)"最短作业优先算法");
}

void fcfs(int m)     //先来先服务算法
{
    int i, iden;
    Copy();
    for (i = 0; i<n; i++)
    {

        p = ready;
        iden = 1;
        do {
            if (p->state == 'W'&&p->reachtime <= times)  iden = 0;
            if (iden)p = p->next;
        } while (p != NULL&&iden);

       if (iden)
        {
            i--;
            printf("\n没有满足要求的进程,需等待");

            times++; // 2根据上下文，填入一条语句

            if (times>100)
            {
               printf("\n时间过长");
               getchar();
            }
        }
        else
        {
            running(p, m);
        }
    }
    final((char*)"先来先服务算法");
}
double cal_hrn(JCB *p) //计算响应比
{
    return 1 + (double)times / (double)p -> needtime;
}
void hrn(int m) //响应比高者优先算法
{
    JCB *min;
    int i,iden;
    Copy();
    running(ready, m);
    for(i=1;i<n;i++)
    {
        p=min=ready;
        iden=1;
        do{
                if(p->state=='W'&&p->reachtime<=times){
                    if(iden)
                    {
                        min=p;
                        iden=0;
                    }
                    else if(cal_hrn(p)>cal_hrn(min)) min=p;
                }
                p = p -> next;
            } while (p != NULL);
        if(iden)
        {
            i--;
            times++;
            if (times>100)
            {
             printf("\nruntime is too long … error");
             getchar();
            }
        }
        else
        {
            running(min, m);
        }
    }
    final((char*)"响应比高者优先算法");
}
int main()
{
    n = inital();
    sjf(n);
    fcfs(n);
    hrn(n);
    system("pause");
    return 0;
}

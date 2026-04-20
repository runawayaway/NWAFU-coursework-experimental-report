#include "stdio.h"
#include <stdlib.h>
#include <conio.h>
#define getpch(type) (type*)malloc(sizeof(type))
#define NULL 0
struct pcb { /* 定义进程控制块PCB */
char name[10];
char state;
int super;
int ntime;
int rtime;
struct pcb* link;
}*ready=NULL,*p;
typedef struct pcb PCB;

sort() /* 建立对进程进行优先级排列函数*/
{
    PCB *first, *second;
    int insert=0;
    if((ready==NULL)||((p->super)>(ready->super))) /*1  若当前运行队列为空或就绪队列中进程的优先级大于运行中进程的优先级                  */
    {
        p->link=ready;
        ready=p;
    }
    else /* 进程比较优先级,插入适当的位置中*/
    {
        first=ready;
        second=first->link;
        while(second!=NULL)
        {
            if((p->super)>(second->super)) /*若插入进程比当前进程优先数大,*/
            { /*插入到当前进程前面*/
                p->link=second;
                first->link=p;
                second=NULL;
                insert=1;
            }

            else /* 插入进程优先数最低,则插入到队尾*/
            {
                first = first->link;
                second=second->link;
            }
        }
        if(insert==0) first->link=p;
    }
}

input() /* 建立进程控制块函数*/
{
    int i,num;
    //clrscr(); /*清屏*/
    system("cls"); /*由于clrscr是包含在<conio.h>中，只能在Borland c++编辑器中运行的函数，所以将其
                    替换为在本机上可以使用且功能相同的system("cls")*/
    printf("\n 请输入进程号?");
    scanf("%d",&num);
    for(i=0;i<num;i++)
    {
        printf("\n 进程号No.%d:\n",i);
        p=getpch(PCB);
        printf("\n 输入进程名:");
        scanf("%s",p->name);
        printf("\n 输入进程优先数:");
        scanf("%d",&p->super);
        printf("\n 输入进程运行时间:");
        scanf("%d",&p->ntime);
        printf("\n");
        p->rtime=0;p->state='w';
        p->link=NULL;
        sort(); /* 调用sort函数*/
    }
}
int space()
{
    int l=0; PCB* pr=ready;
    while(pr!=NULL)
    {
        l++;
        pr=pr->link;
    }
    return(l);
}
disp(PCB * pr) /*建立进程显示函数,用于显示当前进程*/
{
    printf("\n qname \t state \t super \t ndtime \t runtime \n");
    printf("|%s\t",pr->name);
    printf("|%c\t",pr->state);
    printf("|%d\t",pr->super);
    printf("|%d\t",pr->ntime);
    printf("|%d\t",pr->rtime);
    printf("\n");
}

check() /* 建立进程查看函数 */
{
    PCB* pr;
    printf("\n **** 当前正在运行的进程是:%s",p->name); /*显示当前运行进程*/
    disp(p);
    pr=ready;
    printf("\n ****当前就绪队列状态为:\n"); /*显示就绪队列状态*/
    while(pr!=NULL)
    {
        disp(pr);
        pr=pr->link;
    }
}
destroy() /*建立进程撤消函数(进程运行结束,撤消进程)*/
{
    printf("\n 进程 [%s] 已完成.\n",p->name);
    free(p);
}
running() /* 建立进程就绪函数(进程运行时间到,置就绪状态*/
{
    (p->rtime)++;
    if(p->rtime==p->ntime)
    destroy(); /* 调用destroy函数*/
    else
    {
        (p->super)--;
        p->state='w';
        sort(); /*调用sort函数*/
    }
}
main() /*主函数*/
{
    int len,h=0;    /*定义len和h，分别用于存储剩余总进程数和记录总运行次数*/
    char ch;
    input();        /*调用input函数，读入进程信息*/
    len = space();  /*调用space函数，读入的进程总数*/
    while((len!=0)&&(ready!=NULL))/*当剩余进程数不为零且运行队列不为空*/
    {
        ch=getchar();
        h++;                        /*代表执行一次，总执行次数增加*/
        printf("\n The execute number:%d \n",h);    /*输出当前总执行次数*/
        p=ready;                    /*让p变为ready指向的进程，即让当前就绪队列中优先级最高的进程进入
                                    运行队列*/
        ready=p->link;              /*在就绪队列中删去将刚刚进入运行队列的进程，避免重复*/
        p->link=NULL;
        p->state='R';               /*将运行队列中进程的状态置为R，即运行中*/
        check();                    /*调用check，输入运行队列和就绪队列的状态*/
        running();                  /*运行运行队列中的进程一次，将其优先度减一，状态置为W，并对所有进程
                                    按优先度降序排列，即寻找优先度最高的进程*/
        printf("\n 按任一键继续......");
        ch=getchar();
    }
    printf("\n\n 进程已经完成.\n");
    ch=getchar();
    return 0;
}

#include "stdio.h"
#include <stdlib.h>
#include <conio.h>
#define getpch(type) (type*)malloc(sizeof(type))
#define NULL 0
struct pcb { /* 定义进程控制块PCB */
char name[10];
char state;
int ntime;
int rtime;
struct pcb* link;
}*ready=NULL,*p;
typedef struct pcb PCB;

insert() /* 将当前进程插入队列尾部*/
{
    PCB* nail;
    if(ready == NULL) ready = p;
    else
    {
        nail = ready;
        while(nail -> link != NULL)
        {
            nail = nail -> link; /*找到当前队列的尾部*/
        }
        nail -> link = p; /*将当前元素插入队列尾部*/
    }
}

input() /* 建立进程控制块函数*/
{
    int i,num;
    system("cls");
    printf("\n 请输入进程号?");
    scanf("%d",&num);
    for(i=0;i<num;i++)
    {
        printf("\n 进程号No.%d:\n",i);
        p=getpch(PCB);
        printf("\n 输入进程名:");
        scanf("%s",p->name);
        printf("\n 输入进程运行时间:");
        scanf("%d",&p->ntime);
        printf("\n");
        p->rtime=0;p->state='w';
        p->link=NULL;
        insert();
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
    printf("\n qname \t state \t ndtime \t runtime \n");
    printf("|%s\t",pr->name);
    printf("|%c\t",pr->state);
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
        p->state='w';
        insert(); /*调用insert函数*/
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
        running();                  /*运行运行队列中的进程一次，将其优先度减一，状态置为W，并判断其是否完成，若未完成
                                    插入到就绪队列的尾部*/
        printf("\n 按任一键继续......");
        ch=getchar();
    }
    printf("\n\n 进程已经完成.\n");
    ch=getchar();
    return 0;
}

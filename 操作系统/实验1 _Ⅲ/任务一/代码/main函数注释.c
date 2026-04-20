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
        p->link=NULL;				 /*将进入运行队列中的进程指向下一个进程的指针指向null*/
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

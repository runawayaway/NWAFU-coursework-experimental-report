#define _CRT_SECURE_NO_WARNINGS 1
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<conio.h>
#define getpch(type) (type*)malloc(sizeof(type))

// 系统资源定义
#define MAX_MEMORY 10    // 系统总内存(KB)
#define MAX_IO_DEVICES 2   // I/O设备数量

int n;                    // 作业数量
float T1 = 0, T2 = 0;     // 总周转时间和总带权周转时间
int current_time = 0;      // 系统当前时间
int available_memory = MAX_MEMORY;     // 可用内存
int available_io = MAX_IO_DEVICES;     // 可用I/O设备

// 扩展的作业控制块
struct jcb {
    char name[10];        // 作业名称
    int reachtime;        // 到达时间
    int starttime;        // 开始时间
    int needtime;         // 总需要运行时间
    int cpu_time;         // 需要CPU时间
    int io_time;          // 需要I/O时间
    int priority;         // 优先级(数字越小优先级越高)
    int memory;           // 需要内存大小
    int cpu_remaining;    // 剩余CPU时间
    int io_remaining;     // 剩余I/O时间
    int finishtime;       // 完成时间
    float cycletime;      // 周转时间
    float cltime;         // 带权周转时间
    char state;          // 状态: W等待, R运行, F完成, I I/O操作
    struct jcb *next;     // 指针
} *ready = NULL, *cpu_running = NULL, *io_waiting = NULL, *io_running[MAX_IO_DEVICES] = {NULL}, *p, *q;

typedef struct jcb JCB;

// 初始化作业队列
void inital() {
    int i;
    printf("\n输入作业数:");
    scanf("%d", &n);
    for (i = 0; i < n; i++) {
        p = getpch(JCB);
        printf("\n输入作业名:");
        scanf("%s", p->name);
        getchar();
        p->reachtime = i;
        printf("作业默认到达时间:%d", i);
        printf("\n输入作业优先级(数字越小优先级越高):");
        scanf("%d", &p->priority);
        printf("\n输入作业需要的内存(KB):");
        scanf("%d", &p->memory);
        printf("\n输入作业需要的CPU时间:");
        scanf("%d", &p->cpu_time);
        printf("\n输入作业需要的I/O时间:");
        scanf("%d", &p->io_time);
        
        p->needtime = p->cpu_time + p->io_time;
        p->cpu_remaining = p->cpu_time;
        p->io_remaining = p->io_time;
        p->state = 'W';
        p->next = NULL;
        
        // 按优先级插入就绪队列
        if (ready == NULL) {
            ready = p;
        } else {
            JCB *prev = NULL, *curr = ready;
            while (curr != NULL && curr->priority <= p->priority) {
                prev = curr;
                curr = curr->next;
            }
            if (prev == NULL) {
                p->next = ready;
                ready = p;
            } else {
                p->next = curr;
                prev->next = p;
            }
        }
    }
}

// 显示作业信息
void disp(JCB *q) {
    printf("\n作业%s状态:%c\n", q->name, q->state);
    printf("优先级:%d 内存:%dKB\n", q->priority, q->memory);
    printf("CPU时间:%d/%d I/O时间:%d/%d\n", 
           q->cpu_remaining, q->cpu_time, 
           q->io_remaining, q->io_time);
    if (q->state == 'F') {
        printf("开始时间:%d 完成时间:%d\n", q->starttime, q->finishtime);
        printf("周转时间:%f 带权周转时间:%f\n", q->cycletime, q->cltime);
    }
    getchar();
}

// 调度CPU作业
void schedule_cpu() {
    if (cpu_running == NULL && ready != NULL && ready->memory <= available_memory) {
        cpu_running = ready;
        ready = ready->next;
        cpu_running->next = NULL;
        cpu_running->state = 'R';
        cpu_running->starttime = current_time;
        available_memory -= cpu_running->memory;
        printf("\n时间%d: %s开始CPU执行\n", current_time, cpu_running->name);
    }
}

// 调度I/O作业
void schedule_io() {
    for (int i = 0; i < MAX_IO_DEVICES; i++) {
        if (io_running[i] == NULL && io_waiting != NULL) {
            io_running[i] = io_waiting;
            io_waiting = io_waiting->next;
            io_running[i]->next = NULL;
            io_running[i]->state = 'I';
            printf("\n时间%d: %s开始I/O操作(设备%d)\n", 
                   current_time, io_running[i]->name, i);
        }
    }
}

// 运行一个时间单位
void run_time_unit() {
    current_time++;
    
    // 处理CPU运行
    if (cpu_running != NULL) {
        cpu_running->cpu_remaining--;
        printf("\n时间%d: %s正在执行CPU操作(剩余%d)\n", 
               current_time, cpu_running->name, cpu_running->cpu_remaining);
        
        if (cpu_running->cpu_remaining == 0) {
            printf("\n时间%d: %s完成CPU操作\n", current_time, cpu_running->name);
            
            // 如果还有I/O操作，加入I/O等待队列
            if (cpu_running->io_remaining > 0) {
                cpu_running->state = 'W';
                cpu_running->next = io_waiting;
                io_waiting = cpu_running;
                printf("\n时间%d: %s加入I/O等待队列\n", current_time, cpu_running->name);
            } else {
                // 作业完成
                cpu_running->finishtime = current_time;
                cpu_running->cycletime = (float)(cpu_running->finishtime - cpu_running->reachtime);
                cpu_running->cltime = cpu_running->cycletime / cpu_running->needtime;
                cpu_running->state = 'F';
                T1 += cpu_running->cycletime;
                T2 += cpu_running->cltime;
                disp(cpu_running);
                available_memory += cpu_running->memory;
                free(cpu_running);
            }
            cpu_running = NULL;
        }
    }
    
    // 处理I/O操作
    for (int i = 0; i < MAX_IO_DEVICES; i++) {
        if (io_running[i] != NULL) {
            io_running[i]->io_remaining--;
            printf("\n时间%d: %s正在执行I/O操作(设备%d,剩余%d)\n", 
                   current_time, io_running[i]->name, i, io_running[i]->io_remaining);
            
            if (io_running[i]->io_remaining == 0) {
                printf("\n时间%d: %s完成I/O操作(设备%d)\n", 
                       current_time, io_running[i]->name, i);
                
                // 如果还有CPU操作，加入就绪队列
                if (io_running[i]->cpu_remaining > 0) {
                    io_running[i]->state = 'W';
                    io_running[i]->next = ready;
                    ready = io_running[i];
                    printf("\n时间%d: %s加入就绪队列\n", current_time, io_running[i]->name);
                } else {
                    // 作业完成
                    io_running[i]->finishtime = current_time;
                    io_running[i]->cycletime = (float)(io_running[i]->finishtime - io_running[i]->reachtime);
                    io_running[i]->cltime = io_running[i]->cycletime / io_running[i]->needtime;
                    io_running[i]->state = 'F';
                    T1 += io_running[i]->cycletime;
                    T2 += io_running[i]->cltime;
                    disp(io_running[i]);
                    available_memory += io_running[i]->memory;
                    free(io_running[i]);
                }
                io_running[i] = NULL;
            }
        }
    }
}

// 显示系统状态
void show_status() {
    printf("\n当前时间:%d 可用内存:%dKB 可用I/O设备:%d\n", 
           current_time, available_memory, available_io);
    
    printf("\n就绪队列:");
    p = ready;
    while (p != NULL) {
        printf("%s(P%d)->", p->name, p->priority);
        p = p->next;
    }
    printf("NULL\n");
    
    printf("I/O等待队列:");
    p = io_waiting;
    while (p != NULL) {
        printf("%s->", p->name);
        p = p->next;
    }
    printf("NULL\n");
    
    printf("CPU运行:");
    if (cpu_running != NULL) {
        printf("%s\n", cpu_running->name);
    } else {
        printf("NULL\n");
    }
    
    printf("I/O运行:");
    for (int i = 0; i < MAX_IO_DEVICES; i++) {
        if (io_running[i] != NULL) {
            printf("设备%d:%s ", i, io_running[i]->name);
        }
    }
    printf("\n");
}

// 最终统计
void final() {
    float s, t;
    t = T1 / n;
    s = T2 / n;
    getchar();
    printf("\n\n作业已经全部完成!\n");
    printf("\n%d个作业的平均周转时间是：%f", n, t);
    printf("\n%d个作业的平均带权周转时间是：%f\n\n\n", n, s);
}

// 基于优先级的调度算法
void priority_scheduling() {
    system("cls");
    inital();
    
    int completed = 0;
    while (completed < n) {
        schedule_cpu();
        schedule_io();
        show_status();
        run_time_unit();
        
        // 检查完成作业数
        completed = 0;
        if (cpu_running == NULL) completed++;
        for (int i = 0; i < MAX_IO_DEVICES; i++) {
            if (io_running[i] == NULL) completed++;
        }
        
        if (current_time > 100) {
            printf("\n时间过长，可能发生死锁\n");
            break;
        }
        getchar();
    }
    final();
}

void menu() {
    int m;
    system("cls");
    printf("\n\n\t\t*********************************************\t\t\n");
    printf("\t\t\t\t多道程序调度演示\n");
    printf("\t\t*********************************************\t\t\n");
    printf("\n\n\n\t\t\t1.基于优先级的调度算法");
    printf("\n\n\t\t\t\t选择所要操作:");
    scanf("%d", &m);
    switch (m) {
        case 1:
            priority_scheduling();
            getchar();
            system("cls");
            break;
        case 0:
            system("cls");
            break;
        default:
            printf("选择错误,重新选择.");
            getchar();
            system("cls");
            menu();
    }
}

int main() {
    menu();
    system("pause");
    return 0;
}

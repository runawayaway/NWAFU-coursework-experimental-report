#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sched.h>  // 用户空间的sched_yield()（类似schedule()）

// 线程1的任务函数
void *task1(void *arg) {
    for (int i = 0; i < 5; i++) {
        printf("Task 1 is running (Thread ID: %ld)\n", pthread_self());
        sleep(1);           // 休眠1秒
        sched_yield();       // 主动让出CPU（用户空间的schedule()）
    }
    return NULL;
}

// 线程2的任务函数
void *task2(void *arg) {
    for (int i = 0; i < 5; i++) {
        printf("Task 2 is running (Thread ID: %ld)\n", pthread_self());
        sleep(1);
        sched_yield();
    }
    return NULL;
}

int main() {
    pthread_t tid1, tid2;

    // 创建两个线程
    pthread_create(&tid1, NULL, task1, NULL);
    pthread_create(&tid2, NULL, task2, NULL);

    // 等待线程结束
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    printf("All tasks completed.\n");
    return 0;
}


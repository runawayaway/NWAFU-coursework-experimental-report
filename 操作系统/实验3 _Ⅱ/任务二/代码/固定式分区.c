#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#define MAX_PARTITIONS 10   // 系统最大分区数
#define MAX_JOBS 10        // 系统最大作业数
#define INVALID -1         // 无效标志

// 分区结构体
typedef struct {
    int id;             // 分区ID
    int start_addr;     // 起始地址
    int size;           // 分区大小（KB）
    int is_allocated;   // 是否已分配（0-未分配，1-已分配）
    int job_id;         // 分配的作业ID（INVALID表示未分配）
} Partition;

// 作业结构体
typedef struct {
    int id;             // 作业ID
    int size;           // 作业大小（KB）
    int partition_id;   // 分配的分区ID（INVALID表示未分配）
} Job;

Partition partitions[MAX_PARTITIONS];  // 分区表
Job jobs[MAX_JOBS];                   // 作业表

// 初始化10个固定分区
void initialize_partitions() {
    // 初始化10个不同大小的固定分区（单位：KB）
    partitions[0] = (Partition){0, 0, 50, 0, INVALID};
    partitions[1] = (Partition){1, 50, 100, 0, INVALID};
    partitions[2] = (Partition){2, 150, 150, 0, INVALID};
    partitions[3] = (Partition){3, 300, 200, 0, INVALID};
    partitions[4] = (Partition){4, 500, 250, 0, INVALID};
    partitions[5] = (Partition){5, 750, 300, 0, INVALID};
    partitions[6] = (Partition){6, 1050, 350, 0, INVALID};
    partitions[7] = (Partition){7, 1400, 400, 0, INVALID};
    partitions[8] = (Partition){8, 1800, 450, 0, INVALID};
    partitions[9] = (Partition){9, 2250, 500, 0, INVALID};
}

// 初始化作业表
void initialize_jobs() {
    for (int i = 0; i < MAX_JOBS; i++) {
        jobs[i].id = INVALID;
        jobs[i].partition_id = INVALID;
    }
}

// 分配分区（首次适应算法）
int allocate_partition(int job_id, int job_size) {
    if(job_id < 0 || job_id >= MAX_JOBS){
        printf("错误：作业ID必须在0~%d之间\n", MAX_JOBS-1);
        return INVALID;
    }

    if(jobs[job_id].id != INVALID){
        printf("错误：作业%d已分配分区%d\n", job_id, jobs[job_id].partition_id);
        return INVALID;
    }

    // 查找第一个足够大的未分配分区
    for (int i = 0; i < MAX_PARTITIONS; i++) {
        if (!partitions[i].is_allocated && partitions[i].size >= job_size) {
            // 标记分区为已分配
            partitions[i].is_allocated = 1;
            partitions[i].job_id = job_id;

            // 更新作业信息
            jobs[job_id].id = job_id;
            jobs[job_id].size = job_size;
            jobs[job_id].partition_id = partitions[i].id;

            printf("分配成功：作业%d(%dKB) -> 分区%d(%dKB)\n",
                  job_id, job_size, partitions[i].id, partitions[i].size);
            printf("        起始地址：%dKB\n", partitions[i].start_addr);

            return partitions[i].id;
        }
    }

    printf("分配失败：没有适合%dKB的分区\n", job_size);
    return INVALID;
}

// 回收分区
void deallocate_partition(int job_id) {
    if (job_id < 0 || job_id >= MAX_JOBS) {
        printf("错误：无效的作业ID\n");
        return;
    }

    if (jobs[job_id].id == INVALID) {
        printf("错误：作业%d未分配分区\n", job_id);
        return;
    }

    int partition_id = jobs[job_id].partition_id;

    // 重置分区状态
    partitions[partition_id].is_allocated = 0;
    partitions[partition_id].job_id = INVALID;

    // 重置作业信息
    jobs[job_id].id = INVALID;
    jobs[job_id].partition_id = INVALID;

    printf("回收成功：分区%d已释放\n", partition_id);
}

// 显示分区状态
void display_status() {
    printf("\n==================== 分区状态表 ====================\n");
    printf("ID\t起始地址\t大小(KB)\t状态\t\t作业ID\n");
    printf("------------------------------------------------\n");

    int total_free = 0, total_used = 0;

    for (int i = 0; i < MAX_PARTITIONS; i++) {
        printf("%d\t%d\t\t%d\t\t",
               partitions[i].id,
               partitions[i].start_addr,
               partitions[i].size);

        if (partitions[i].is_allocated) {
            printf("已分配(作业%d)\t%d\n", partitions[i].job_id, partitions[i].job_id);
            total_used += partitions[i].size;
        } else {
            printf("空闲\t\t-\n");
            total_free += partitions[i].size;
        }
    }

    printf("\n总空闲空间：%dKB\t总已用空间：%dKB\n", total_free, total_used);

    printf("\n==================== 作业分配表 ====================\n");
    printf("作业ID\t大小(KB)\t分配的分区ID\n");
    printf("------------------------------------------------\n");

    int active_jobs = 0;
    for (int i = 0; i < MAX_JOBS; i++) {
        if (jobs[i].id != INVALID) {
            printf("%d\t%d\t\t%d\n", jobs[i].id, jobs[i].size, jobs[i].partition_id);
            active_jobs++;
        }
    }

    if(active_jobs == 0){
        printf("当前没有活跃作业\n");
    }
    printf("\n");
}

// 显示初始化分区配置
void show_partition_config() {
    printf("\n系统初始分区配置（共%d个固定分区）：\n", MAX_PARTITIONS);
    printf("ID\t起始地址(KB)\t大小(KB)\n");
    printf("-----------------------------\n");
    for(int i=0; i<MAX_PARTITIONS; i++){
        printf("%d\t%d\t\t%d\n",
              partitions[i].id,
              partitions[i].start_addr,
              partitions[i].size);
    }
    printf("注：内存总容量 = %dKB\n\n",
          partitions[MAX_PARTITIONS-1].start_addr + partitions[MAX_PARTITIONS-1].size);
}

int main() {
    initialize_partitions();
    initialize_jobs();

    show_partition_config();

    int choice;
    while (1) {
        printf("\n==== 固定分区存储管理系统 ====\n");
        printf("1. 分配分区\n");
        printf("2. 回收分区\n");
        printf("3. 显示状态\n");
        printf("0. 退出\n");
        printf("请选择操作: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: {
                int job_id, job_size;
                printf("输入作业ID(0-%d)和所需大小(KB): ", MAX_JOBS-1);
                scanf("%d %d", &job_id, &job_size);
                allocate_partition(job_id, job_size);
                break;
            }

            case 2: {
                int job_id;
                printf("输入要回收的作业ID(0-%d): ", MAX_JOBS-1);
                scanf("%d", &job_id);
                deallocate_partition(job_id);
                break;
            }

            case 3:
                display_status();
                break;

            case 0:
                printf("系统退出\n");
                exit(0);

            default:
                printf("无效输入！请选择0-3\n");
        }
    }
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#define MAX_SEG 5    // 最大段数
#define MAX_PAGE 10  // 每段最大页数
#define PAGE_SIZE 12 // 页大小
#define SEG_SIZE 1024 // 段大小

// 段表项结构
struct {
    int valid;      // 有效位
    int page_count; // 页表长度
    int *page_table;// 页表指针
    int page_size;
} seg_table[MAX_SEG];

// 初始化段表
void init_seg_table() {
    for (int i = 0; i < MAX_SEG; i++) {
        seg_table[i].valid = 0;
        seg_table[i].page_table = NULL;
    }
}

// 地址转换
void address_translation() {
    int s, d; // 段号, 段内偏移
    printf("输入段号和页号: ");
    scanf("%d %d", &s, &d);

    // 计算物理地址
    int physical_addr = s * SEG_SIZE + d * PAGE_SIZE;
    printf("物理地址: %d", physical_addr);
}

int main() {
    init_seg_table();

    // 简单初始化两个段
    seg_table[0].valid = 1;
    seg_table[0].page_count = 3;
    seg_table[0].page_table = (int*)malloc(3 * sizeof(int));
    seg_table[0].page_table[0] = 5; // 第0页装入5
    seg_table[0].page_table[1] = 8; // 第1页装入8
    seg_table[0].page_table[2] = -1; // 第2页未装入
    seg_table[2].valid = 1;
    seg_table[2].page_count = 4;
    seg_table[2].page_table = (int*)malloc(4 * sizeof(int));
    seg_table[2].page_table[0] = 5; // 第0页装入5
    seg_table[2].page_table[1] = 8; // 第1页装入8
    seg_table[2].page_table[2] = 7; // 第2页未装入

    while(1) {
        printf("\n1. 地址转换\n");
        printf("0. 退出\n");
        printf("选择: ");

        int choice;
        scanf("%d", &choice);

        switch(choice) {
            case 1:
                address_translation();
                break;
            case 0:
                exit(0);
            default:
                printf("无效选择!\n");
        }
    }
    return 0;
}

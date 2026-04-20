#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_NAME_LEN 20      // 文件/目录名最大长度
#define MAX_CHILDREN 10      // 每个目录最大子项数
#define MAX_DEPTH 5          // 目录最大深度

// 文件保护级别枚举
typedef enum {
    PUBLIC,     // 公开
    PRIVATE,    // 私有
    PROTECTED   // 受保护
} ProtectionLevel;

// 文件系统节点结构
typedef struct TreeNode {
    char name[MAX_NAME_LEN];            // 节点名称
    bool isDirectory;                    // 是否为目录
    ProtectionLevel protection;          // 保护级别
    struct TreeNode* parent;             // 父节点指针
    struct TreeNode* children[MAX_CHILDREN]; // 子节点数组
    int childCount;                      // 子节点数量
} TreeNode;

TreeNode* root = NULL;  // 根目录
TreeNode* currentDir = NULL; // 当前目录

// 初始化文件系统
void initializeFS() {
    root = (TreeNode*)malloc(sizeof(TreeNode));
    strcpy(root->name, "root");
    root->isDirectory = true;
    root->protection = PUBLIC;
    root->parent = NULL;
    root->childCount = 0;
    currentDir = root;
    
    // 创建一些初始文件和目录
    TreeNode* dir1 = (TreeNode*)malloc(sizeof(TreeNode));
    strcpy(dir1->name, "documents");
    dir1->isDirectory = true;
    dir1->protection = PROTECTED;
    dir1->parent = root;
    dir1->childCount = 0;
    root->children[root->childCount++] = dir1;

    TreeNode* file1 = (TreeNode*)malloc(sizeof(TreeNode));
    strcpy(file1->name, "readme.txt");
    file1->isDirectory = false;
    file1->protection = PUBLIC;
    file1->parent = root;
    file1->childCount = 0;
    root->children[root->childCount++] = file1;

    printf("文件系统初始化完成！\n");
}

// 递归显示目录树
void showTree(TreeNode* node, int depth) {
    // 缩进显示层级关系
    for (int i = 0; i < depth; i++) {
        printf("│   ");
    }
    
    // 显示节点信息
    printf("├── %s", node->name);
    if (node->isDirectory) {
        printf("/");
    }
    
    // 显示保护级别
    printf(" [");
    switch(node->protection) {
        case PUBLIC:    printf("公开"); break;
        case PRIVATE:   printf("私有"); break;
        case PROTECTED: printf("受保护"); break;
    }
    printf("]\n");
    
    // 递归显示子节点
    for (int i = 0; i < node->childCount; i++) {
        showTree(node->children[i], depth + 1);
    }
}

// 改变文件保护级别
void changeProtection() {
    char name[MAX_NAME_LEN];
    int level;
    
    printf("当前目录内容:\n");
    for (int i = 0; i < currentDir->childCount; i++) {
        printf("%d. %s", i+1, currentDir->children[i]->name);
        if (currentDir->children[i]->isDirectory) printf("/");
        printf("\n");
    }
    
    printf("请输入要修改的文件/目录编号: ");
    int choice;
    scanf("%d", &choice);
    if (choice < 1 || choice > currentDir->childCount) {
        printf("无效选择！\n");
        return;
    }
    
    TreeNode* target = currentDir->children[choice-1];
    
    printf("请选择新的保护级别:\n");
    printf("1. 公开\n2. 私有\n3. 受保护\n选择: ");
    scanf("%d", &level);
    
    if (level < 1 || level > 3) {
        printf("无效选择！\n");
        return;
    }
    
    target->protection = level - 1; // 转换为枚举值
    printf("保护级别修改成功！\n");
}

// 创建新文件或目录
void createNode() {
    char name[MAX_NAME_LEN];
    int type;
    
    printf("请输入名称: ");
    scanf("%s", name);
    
    // 检查名称是否已存在
    for (int i = 0; i < currentDir->childCount; i++) {
        if (strcmp(currentDir->children[i]->name, name) == 0) {
            printf("名称已存在！\n");
            return;
        }
    }
    
    printf("请选择类型 (1.文件 2.目录): ");
    scanf("%d", &type);
    
    printf("请选择保护级别 (1.公开 2.私有 3.受保护): ");
    int level;
    scanf("%d", &level);
    
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
    strcpy(newNode->name, name);
    newNode->isDirectory = (type == 2);
    newNode->protection = level - 1;
    newNode->parent = currentDir;
    newNode->childCount = 0;
    
    currentDir->children[currentDir->childCount++] = newNode;
    printf("创建成功！\n");
}

// 切换当前目录
void changeDirectory() {
    printf("当前目录内容:\n");
    int dirCount = 0;
    int dirIndices[MAX_CHILDREN];
    
    for (int i = 0; i < currentDir->childCount; i++) {
        if (currentDir->children[i]->isDirectory) {
            printf("%d. %s/\n", dirCount+1, currentDir->children[i]->name);
            dirIndices[dirCount++] = i;
        }
    }
    printf("0. 返回上级目录\n");
    
    if (dirCount == 0 && currentDir == root) {
        printf("没有子目录可切换！\n");
        return;
    }
    
    printf("选择目录编号: ");
    int choice;
    scanf("%d", &choice);
    
    if (choice == 0) {
        if (currentDir != root) {
            currentDir = currentDir->parent;
            printf("已切换到 %s\n", currentDir->name);
        } else {
            printf("已经是根目录！\n");
        }
    } else if (choice >= 1 && choice <= dirCount) {
        currentDir = currentDir->children[dirIndices[choice-1]];
        printf("已切换到 %s\n", currentDir->name);
    } else {
        printf("无效选择！\n");
    }
}

// 主菜单
void menu() {
    printf("\n\n\t\t※§ 文件系统 §※\n");
    printf("\n\t当前目录: %s\n", currentDir->name);
    printf("\n\t1. 显示目录树");
    printf("\n\t2. 修改文件保护级别");
    printf("\n\t3. 创建文件/目录");
    printf("\n\t4. 切换目录");
    printf("\n\t5. 退出系统");
    printf("\n\n\t请选择操作: ");
}

int main() {
    initializeFS();
    
    int choice;
    while (1) {
        menu();
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                printf("\n文件系统目录结构:\n");
                showTree(root, 0);
                break;
            case 2:
                changeProtection();
                break;
            case 3:
                createNode();
                break;
            case 4:
                changeDirectory();
                break;
            case 5:
                printf("\n感谢使用文件系统！\n");
                exit(0);
            default:
                printf("无效选择！\n");
        }
    }
    return 0;
}

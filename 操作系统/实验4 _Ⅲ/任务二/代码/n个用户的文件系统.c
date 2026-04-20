#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_USERS 10        // 最大用户数
#define MAX_FILES 10        // 每个用户最大文件数
#define MAX_NAME_LEN 20     // 名称最大长度

// 文件保护级别
typedef enum {
    PUBLIC,     // 公开
    PRIVATE,    // 私有
    PROTECTED   // 受保护
} ProtectionLevel;

// 文件结构
typedef struct {
    char name[MAX_NAME_LEN];    // 文件名
    char content[100];          // 文件内容
    ProtectionLevel protect;    // 保护级别
    bool isOpen;                // 是否打开
    int ownerId;                // 所有者ID
} File;

// 用户结构
typedef struct {
    int id;                     // 用户ID
    char name[MAX_NAME_LEN];    // 用户名
    File files[MAX_FILES];      // 文件数组
    int fileCount;              // 文件计数
    File* openedFile;           // 当前打开的文件
} User;

User users[MAX_USERS];          // 用户数组
int userCount = 0;              // 用户计数
int currentUserId = -1;         // 当前登录用户ID

// 初始化系统
void initializeSystem() {
    userCount = 0;
    currentUserId = -1;
    printf("系统初始化完成！\n");
}

// 创建用户
void createUser() {
    if (userCount >= MAX_USERS) {
        printf("用户数量已达上限！\n");
        return;
    }

    printf("请输入用户名: ");
    scanf("%s", users[userCount].name);

    users[userCount].id = userCount;
    users[userCount].fileCount = 0;
    users[userCount].openedFile = NULL;

    printf("用户 %s 创建成功，ID为 %d\n", users[userCount].name, userCount);
    userCount++;
}

// 用户登录
void login() {
    if (currentUserId != -1) {
        printf("请先注销当前用户！\n");
        return;
    }

    printf("请输入用户ID: ");
    int id;
    scanf("%d", &id);

    if (id < 0 || id >= userCount) {
        printf("无效用户ID！\n");
        return;
    }

    currentUserId = id;
    printf("用户 %s 登录成功！\n", users[currentUserId].name);
}

// 用户注销
void logout() {
    if (currentUserId == -1) {
        printf("没有用户登录！\n");
        return;
    }

    // 关闭已打开的文件
    if (users[currentUserId].openedFile != NULL) {
        users[currentUserId].openedFile->isOpen = false;
        users[currentUserId].openedFile = NULL;
    }

    printf("用户 %s 已注销\n", users[currentUserId].name);
    currentUserId = -1;
}

// 创建文件
void createFile() {
    if (currentUserId == -1) {
        printf("请先登录！\n");
        return;
    }

    User* currentUser = &users[currentUserId];
    if (currentUser->fileCount >= MAX_FILES) {
        printf("文件数量已达上限！\n");
        return;
    }

    printf("请输入文件名: ");
    char name[MAX_NAME_LEN];
    scanf("%s", name);

    // 检查文件名是否已存在
    for (int i = 0; i < currentUser->fileCount; i++) {
        if (strcmp(currentUser->files[i].name, name) == 0) {
            printf("文件名已存在！\n");
            return;
        }
    }

    printf("请选择保护级别 (1.公开 2.私有 3.受保护): ");
    int level;
    scanf("%d", &level);

    File* newFile = &currentUser->files[currentUser->fileCount];
    strcpy(newFile->name, name);
    newFile->protect = (ProtectionLevel)(level - 1);
    newFile->isOpen = false;
    newFile->ownerId = currentUserId;
    strcpy(newFile->content, "");  // 初始内容为空

    currentUser->fileCount++;
    printf("文件 %s 创建成功！\n", name);
}

// 删除文件
void deleteFile() {
    if (currentUserId == -1) {
        printf("请先登录！\n");
        return;
    }

    User* currentUser = &users[currentUserId];
    if (currentUser->fileCount == 0) {
        printf("没有文件可删除！\n");
        return;
    }

    printf("当前用户的文件列表:\n");
    for (int i = 0; i < currentUser->fileCount; i++) {
        printf("%d. %s [", i+1, currentUser->files[i].name);
        switch(currentUser->files[i].protect) {
            case PUBLIC: printf("公开"); break;
            case PRIVATE: printf("私有"); break;
            case PROTECTED: printf("受保护"); break;
        }
        printf("]\n");
    }

    printf("请选择要删除的文件编号: ");
    int choice;
    scanf("%d", &choice);

    if (choice < 1 || choice > currentUser->fileCount) {
        printf("无效选择！\n");
        return;
    }

    // 如果要删除的文件已打开，先关闭
    if (currentUser->files[choice-1].isOpen) {
        currentUser->files[choice-1].isOpen = false;
        if (currentUser->openedFile == &currentUser->files[choice-1]) {
            currentUser->openedFile = NULL;
        }
    }

    // 移动数组元素填补删除位置
    for (int i = choice-1; i < currentUser->fileCount-1; i++) {
        currentUser->files[i] = currentUser->files[i+1];
    }

    currentUser->fileCount--;
    printf("文件删除成功！\n");
}

// 打开文件
void openFile() {
    if (currentUserId == -1) {
        printf("请先登录！\n");
        return;
    }

    User* currentUser = &users[currentUserId];
    if (currentUser->fileCount == 0) {
        printf("没有文件可打开！\n");
        return;
    }

    // 检查是否已有打开的文件
    if (currentUser->openedFile != NULL) {
        printf("请先关闭当前打开的文件！\n");
        return;
    }

    printf("当前用户的文件列表:\n");
    for (int i = 0; i < currentUser->fileCount; i++) {
        printf("%d. %s [", i+1, currentUser->files[i].name);
        switch(currentUser->files[i].protect) {
            case PUBLIC: printf("公开"); break;
            case PRIVATE: printf("私有"); break;
            case PROTECTED: printf("受保护"); break;
        }
        printf("]\n");
    }

    printf("请选择要打开的文件编号: ");
    int choice;
    scanf("%d", &choice);

    if (choice < 1 || choice > currentUser->fileCount) {
        printf("无效选择！\n");
        return;
    }

    File* selectedFile = &currentUser->files[choice-1];
    selectedFile->isOpen = true;
    currentUser->openedFile = selectedFile;
    printf("文件 %s 已打开\n", selectedFile->name);
}

// 关闭文件
void closeFile() {
    if (currentUserId == -1) {
        printf("请先登录！\n");
        return;
    }

    User* currentUser = &users[currentUserId];
    if (currentUser->openedFile == NULL) {
        printf("没有打开的文件！\n");
        return;
    }

    currentUser->openedFile->isOpen = false;
    printf("文件 %s 已关闭\n", currentUser->openedFile->name);
    currentUser->openedFile = NULL;
}

// 读取文件
void readFile() {
    if (currentUserId == -1) {
        printf("请先登录！\n");
        return;
    }

    User* currentUser = &users[currentUserId];
    if (currentUser->openedFile == NULL) {
        printf("请先打开文件！\n");
        return;
    }

    File* file = currentUser->openedFile;

    // 检查读取权限
    if (file->protect == PRIVATE && file->ownerId != currentUserId) {
        printf("无读取权限！\n");
        return;
    }

    printf("文件内容:\n%s\n", file->content);
}

// 写入文件
void writeFile() {
    if (currentUserId == -1) {
        printf("请先登录！\n");
        return;
    }

    User* currentUser = &users[currentUserId];
    if (currentUser->openedFile == NULL) {
        printf("请先打开文件！\n");
        return;
    }

    File* file = currentUser->openedFile;

    // 检查写入权限
    if (file->protect != PUBLIC && file->ownerId != currentUserId) {
        printf("无写入权限！\n");
        return;
    }

    printf("请输入要写入的内容(不超过100字符): ");
    char content[100];
    getchar(); // 清除缓冲区
    fgets(content, sizeof(content), stdin);
    content[strcspn(content, "\n")] = 0; // 去除换行符

    strcpy(file->content, content);
    printf("内容已写入文件\n");
}

// 修改文件保护级别
void changeProtection() {
    if (currentUserId == -1) {
        printf("请先登录！\n");
        return;
    }

    User* currentUser = &users[currentUserId];
    if (currentUser->fileCount == 0) {
        printf("没有文件可修改！\n");
        return;
    }

    printf("当前用户的文件列表:\n");
    for (int i = 0; i < currentUser->fileCount; i++) {
        printf("%d. %s [当前级别:", i+1, currentUser->files[i].name);
        switch(currentUser->files[i].protect) {
            case PUBLIC: printf("公开"); break;
            case PRIVATE: printf("私有"); break;
            case PROTECTED: printf("受保护"); break;
        }
        printf("]\n");
    }

    printf("请选择要修改的文件编号: ");
    int choice;
    scanf("%d", &choice);

    if (choice < 1 || choice > currentUser->fileCount) {
        printf("无效选择！\n");
        return;
    }

    // 检查是否为文件所有者
    if (currentUser->files[choice-1].ownerId != currentUserId) {
        printf("只有文件所有者可以修改保护级别！\n");
        return;
    }

    printf("请选择新的保护级别 (1.公开 2.私有 3.受保护): ");
    int level;
    scanf("%d", &level);

    if (level < 1 || level > 3) {
        printf("无效选择！\n");
        return;
    }

    currentUser->files[choice-1].protect = (ProtectionLevel)(level - 1);
    printf("保护级别修改成功！\n");
}

// 显示用户文件列表
void listFiles() {
    if (currentUserId == -1) {
        printf("请先登录！\n");
        return;
    }

    User* currentUser = &users[currentUserId];
    printf("用户 %s 的文件列表:\n", currentUser->name);

    if (currentUser->fileCount == 0) {
        printf("(无文件)\n");
        return;
    }

    for (int i = 0; i < currentUser->fileCount; i++) {
        printf("%d. %s [", i+1, currentUser->files[i].name);
        switch(currentUser->files[i].protect) {
            case PUBLIC: printf("公开"); break;
            case PRIVATE: printf("私有"); break;
            case PROTECTED: printf("受保护"); break;
        }
        printf("] %s\n", currentUser->files[i].isOpen ? "(已打开)" : "");
    }
}

// 主菜单
void mainMenu() {
    printf("\n\n\t\t※§ 多用户文件系统 §※\n");
    if (currentUserId != -1) {
        printf("\n\t当前用户: %s (ID:%d)", users[currentUserId].name, currentUserId);
        if (users[currentUserId].openedFile != NULL) {
            printf("\n\t已打开文件: %s", users[currentUserId].openedFile->name);
        }
    }
    printf("\n\n\t1. 创建用户");
    printf("\n\t2. 用户登录");
    printf("\n\t3. 用户注销");
    printf("\n\t4. 创建文件");
    printf("\n\t5. 删除文件");
    printf("\n\t6. 打开文件");
    printf("\n\t7. 关闭文件");
    printf("\n\t8. 读取文件");
    printf("\n\t9. 写入文件");
    printf("\n\t10. 修改文件保护级别");
    printf("\n\t11. 显示文件列表");
    printf("\n\t12. 退出系统");
    printf("\n\n\t请选择操作: ");
}

int main() {
    initializeSystem();

    int choice;
    while (1) {
        mainMenu();
        scanf("%d", &choice);

        switch (choice) {
            case 1: createUser(); break;
            case 2: login(); break;
            case 3: logout(); break;
            case 4: createFile(); break;
            case 5: deleteFile(); break;
            case 6: openFile(); break;
            case 7: closeFile(); break;
            case 8: readFile(); break;
            case 9: writeFile(); break;
            case 10: changeProtection(); break;
            case 11: listFiles(); break;
            case 12:
                printf("\n感谢使用文件系统！\n");
                exit(0);
            default:
                printf("无效选择！\n");
        }
    }

    return 0;
}

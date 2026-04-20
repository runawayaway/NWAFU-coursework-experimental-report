#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char *argv[]) {
if (argc < 3) {
fprintf(stderr, "Usage: %s <command> [args...]\n", argv[0]);
return EXIT_FAILURE;
}

// 构造命令字符串（argv[1] 及其后续参数）
char *command = argv[2];
char **cmd_args = &argv[2]; // 指向命令及其参数的指针数组

pid_t pid = fork(); // 创建子进程

if (pid < 0) {
perror("fork");
return EXIT_FAILURE;
} else if (pid == 0) {
// 子进程：执行命令
execvp(command, cmd_args); // 使用 execvp 自动搜索 PATH
perror("execvp"); // 如果 execvp 失败才会执行到这里
exit(EXIT_FAILURE);
} else {
// 父进程：等待子进程结束
int status;
waitpid(pid, &status, 0);

if (WIFEXITED(status)) {
return WEXITSTATUS(status); // 返回子进程的退出状态
} else {
return EXIT_FAILURE; // 子进程异常终止
}
}
}

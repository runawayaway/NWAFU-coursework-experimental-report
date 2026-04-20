#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
int main()
{
	pid_t pid;
	pid = fork();
	if(pid == 0)
	{
		printf("由子进程执行\n");
		execl("/bin/ls", "ls", "-l", ".", NULL);
	}
	return 0;
}

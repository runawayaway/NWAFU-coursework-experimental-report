#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
int main()
{
	int file;
	if((file = open("hello", O_CREAT | O_TRUNC | O_WRONLY, 0777)) < 0)
	{
		perror("打开文件错误");
		exit(1);
	}
	else
	{
		char*msg = "Linux下C软件设计";
		write(file, msg, strlen(msg));
	}
	if(close(file) < 0)
	{
		perror("关闭文件出错");
		exit(1);
	}
	return 0;
}

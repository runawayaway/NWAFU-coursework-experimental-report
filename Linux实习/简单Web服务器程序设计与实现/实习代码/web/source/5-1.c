#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
int main()
{
	int file;
	if((file = open("pass",O_WRONLY)) < 0)
	{
		file = open("pass", O_CREAT|O_WRONLY, 0400);
	}
	if(close(file)<0)
	{
		perror("关闭文件出错");
		exit(1);
	}
	system("ls pass -l");
	return 0;
}

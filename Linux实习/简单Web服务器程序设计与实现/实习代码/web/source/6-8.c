#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/resource.h>
#include<dirent.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
int main()
{
	pid_t pid;
	pid = fork();
	if(pid > 0) exit(0);
	setsid();
	chdir("/");
	umask(0);
	for(int i = 0; i < 1024; i++) close(i);
	DIR *dir;
	dir = opendir ("/var/spool/mail/s2023013472");
	if(dir == NULL)
	{
		printf("用户的邮件目录不存在");
	}
	else
	{
		closedir(dir);
		while(1)
		{
			dir = opendir("/var/spool/mail/s2023013472");
			struct dirent* dnt;
			int cnt = 0;
			while((dnt = readdir(dir)) != NULL)
			{
				if(!strcmp(".", dnt -> d_name) &&  !strcmp("..", dnt -> d_name)) cnt++;
			}
			if(cnt != 0)
			{
				//执行相关操作，如让机箱上的小喇叭发出声音
			}
			closedir(dir);
			sleep(60);
		}
	}
	return 0;
}

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
int main()
{
        char *envp[]={"PATH=/tmp", "USER=liu", NULL};
	char *arg[]={"env", NULL};
	execve("/bin/env", arg, envp);
        return 1;
}


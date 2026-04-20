#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
int main()
{
	char *envp[]={"PATH=/tmp", "USER=liu", NULL};
	execle("/bin/env", "env", NULL, envp);
	return 1;
}

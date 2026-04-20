#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
int main()
{
        char *arg[]={"ps", "-ef", NULL};
        execvp("ps", arg);
        return 1;
}


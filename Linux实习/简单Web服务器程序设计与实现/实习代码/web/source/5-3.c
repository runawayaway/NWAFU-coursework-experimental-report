#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
int main()
{
	int file, len;
	file = open("/etc/passwd", O_RDONLY);
	if(file < 0) exit(1);
	char* buf;
	while((len = read(file, buf, 20)) > 0)
	{
		write(STDOUT_FILENO, buf, len);
	}
	close(file);
	return 0;
}

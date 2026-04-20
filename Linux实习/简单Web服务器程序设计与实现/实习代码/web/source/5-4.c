#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
int main()
{
        int file_in, file_out, len;
	char* buf;
        if((file_out = open("passed",O_WRONLY | O_APPEND)) < 0)
        {
		file_out = open("passed", O_CREAT|O_WRONLY, 0777);
        }
        file_in = open("/etc/passwd", O_RDONLY);
        if(file_in < 0 || file_out < 0) exit(1);
        while((len = read(file_in, buf, 20)) > 0)
        {
                write(file_out, buf, len);
        }
        close(file_in);
	close(file_out);
        return 0;
}


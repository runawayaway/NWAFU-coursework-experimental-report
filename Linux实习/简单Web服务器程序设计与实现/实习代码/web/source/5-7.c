#include<unistd.h>
#include<stdlib.h>
int main()
{
	symlink("/bin/ls","ls1");
	link("/bin/ls", "ls2");
	system("ls ls1 -l");
	system("ls ls2 -l");
	system("ls /bin/ls -l");
	return 0;
}

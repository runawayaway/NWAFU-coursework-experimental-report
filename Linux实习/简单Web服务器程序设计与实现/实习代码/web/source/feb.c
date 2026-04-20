#include<stdio.h>
int feb[30];
int main()
{
	int n = 20;
	feb[1] = 1;
	feb[2] = 1;
	for(int i = 1; i < n; i++)
	{
		if(feb[i] == 0)
		{
			feb[i] = feb[i - 1] + feb[i - 2];
		}
		printf("%d ", feb[i]);
	}
	return 0;
}

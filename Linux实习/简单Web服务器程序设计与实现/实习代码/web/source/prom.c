#include<stdio.h>
void insert(int a[], int n);
void quicksort(int a[], int first, int last);
int main()
{
	int a[10];
	for(int i = 0; i < 10; i++) 
	{
		a[i] = 9 - i;
		printf("%d ", a[i]);
	}
	printf("\n");
	printf("insert:");
	insert(a, 10);
	for(int i = 0; i < 10; i++) printf("%d ", a[i]);
	printf("\n");
	for(int i = 0; i < 10; i++)
        {
                a[i] = 9 - i;
                printf("%d ", a[i]);
        }
        printf("\n");
        printf("quicksort:");
	quicksort(a, 0, 9);
	for(int i = 0; i < 10; i++) printf("%d ", a[i]);
        printf("\n");
	return 0;
}


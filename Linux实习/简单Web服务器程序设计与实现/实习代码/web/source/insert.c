#include<stdio.h>
void insert(int a[], int n)
{
	for(int i = 1; i < n; i++)
	{
		for(int j = i; j > 0 && a[j - 1] > a[j]; j--)
		{
			int t = a[j];
			a[j] = a[j - 1];
			a[j - 1] = t;
		}
	}
}	

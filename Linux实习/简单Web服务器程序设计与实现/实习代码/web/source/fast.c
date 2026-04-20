#include<stdio.h>
void quicksort(int a[], int first, int last) 
{
	int i, j, p, t;
	if (first < last) 
	{
		p = first;
		i = first;
		j = last;
		while (i < j) 
		{
			while (a[i] <= a[p] && i<last) i++;
			while (a[j] > a[p]) j--;
			if(i < j) 
			{
				t = a[i];
				a[i] = a[j];
				a[j] = t;
			}
		}
		t = a[p];
		a[p] = a[j];
		a[j] = t;
		quicksort(a, first, j - 1);
		quicksort(a, j + 1, last);
	}
}

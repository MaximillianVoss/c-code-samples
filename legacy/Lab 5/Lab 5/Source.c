#include <stdio.h>
#include <stdlib.h>
#include <corecrt_malloc.h>

void Print(int  ** a,int N, int K)
{
	for (int i = 0;i < N;i++)
	{
		for (int j = 0;j < K;j++)
		{
			printf("%4i ", a[i][j]);
		}
		printf("\n");
	}


}
void Sort(int *a, int l)
{
	for (int i = 0;i < l - 1;i++)
	{
		for (int j = 0;j < l - i - 1;j++)
		{
			if (a[j] > a[j + 1])
			{
				int temp = a[j];
				a[j] = a[j + 1];
				a[j + 1] = temp;
			}
		}
	}
}

void main()
{
	int N, K;
	scanf_s("%i", &N);
	scanf_s("%i", &K);

	int **a = (int **)malloc(N * sizeof(int *));
	for (int i = 0; i < N; i++)
		a[i] = (int *)malloc(K * sizeof(int));

	srand(time(NULL));
	for (int i = 0;i < N;i++)
		for (int j = 0;j < K;j++)
			a[i][j] = rand() % 100;
	printf("Matrix:\n");
	Print(a, K, N);

	for (int i = 0;i < N;i++)
		Sort(a[i], K);


	printf("Matrix sorted:\n");
	Print(a, K, N);

	getchar();
	getchar();
}
#pragma once
#include "stdafx.h"

/// <summary>
/// Примеры чтения и записи текстовых и бинарных файлов.
/// </summary>
void Read(void)
{
	FILE* fptr = fopen("newprogram.txt", "w");
	if (fptr != NULL)
	{
		fprintf(fptr, "Code samples C\n");
		fclose(fptr);
	}
}

int ReadEx(void)
{
	int num;
	FILE* fptr = fopen("program.txt", "w");

	if (fptr == NULL)
	{
		printf("Error!");
		return 1;
	}

	printf("Enter num: ");
	if (scanf("%d", &num) != 1)
	{
		fclose(fptr);
		return 1;
	}

	fprintf(fptr, "%d", num);
	fclose(fptr);

	return 0;
}

int WriteEx(void)
{
	int num;
	FILE* fptr = fopen("program.txt", "r");

	if (fptr == NULL)
	{
		printf("Error! opening file");
		return 1;
	}

	if (fscanf(fptr, "%d", &num) != 1)
	{
		fclose(fptr);
		return 1;
	}

	printf("Value of n=%d", num);
	fclose(fptr);

	return 0;
}

typedef struct
{
	int n1;
	int n2;
	int n3;
} ThreeNum;

int WriteBin(void)
{
	FILE* fptr = fopen("program.bin", "wb");
	if (fptr == NULL)
	{
		printf("Error! opening file");
		return 1;
	}

	for (int n = 1; n < 5; ++n)
	{
		ThreeNum num = { n, 5 * n, 5 * n + 1 };
		fwrite(&num, sizeof(ThreeNum), 1, fptr);
	}
	fclose(fptr);

	return 0;
}

int ReadBin(void)
{
	FILE* fptr = fopen("program.bin", "rb");
	if (fptr == NULL)
	{
		printf("Error! opening file");
		return 1;
	}

	for (int n = 1; n < 5; ++n)
	{
		ThreeNum num;
		if (fread(&num, sizeof(ThreeNum), 1, fptr) != 1)
		{
			fclose(fptr);
			return 1;
		}
		printf("n1: %d\tn2: %d\tn3: %d\n", num.n1, num.n2, num.n3);
	}
	fclose(fptr);

	return 0;
}

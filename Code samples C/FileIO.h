#pragma once
#include "stdafx.h"

/// <summary>
/// чтение txt файла
/// подробнее про флаги:
/// https://www.programiz.com/c-programming/c-file-input-output
/// </summary>
void Read() {
	FILE *fptr;
	fopen("E:\\cprogram\\newprogram.txt", "w");
}

int ReadEx()
{
	int num;
	FILE *fptr;
	fptr = fopen("C:\\program.txt", "w");

	if (fptr == NULL)
	{
		printf("Error!");
		exit(1);
	}

	printf("Enter num: ");
	scanf("%d", &num);

	fprintf(fptr, "%d", num);
	fclose(fptr);

	return 0;
}
int WriteEx()
{
	int num;
	FILE *fptr;

	if ((fptr = fopen("C:\\program.txt", "r")) == NULL) {
		printf("Error! opening file");

		// Program exits if the file pointer returns NULL.
		exit(1);
	}

	fscanf(fptr, "%d", &num);

	printf("Value of n=%d", num);
	fclose(fptr);

	return 0;
}

struct threeNum
{
	int n1, n2, n3;
};
int WriteBin()
{
	int n;
	struct threeNum num;
	FILE *fptr;

	if ((fptr = fopen("C:\\program.bin", "wb")) == NULL) {
		printf("Error! opening file");

		// Program exits if the file pointer returns NULL.
		exit(1);
	}

	for (n = 1; n < 5; ++n)
	{
		/*num.n1 = n;
		num.n2 = 5n;
		num.n3 = 5n + 1;*/
		fwrite(&num, sizeof(struct threeNum), 1, fptr);
	}
	fclose(fptr);

	return 0;
}
int ReadBin()
{
	int n;
	struct threeNum num;
	FILE *fptr;

	if ((fptr = fopen("C:\\program.bin", "rb")) == NULL) {
		printf("Error! opening file");

		// Program exits if the file pointer returns NULL.
		exit(1);
	}

	for (n = 1; n < 5; ++n)
	{
		fread(&num, sizeof(struct threeNum), 1, fptr);
		printf("n1: %d\tn2: %d\tn3: %d", num.n1, num.n2, num.n3);
	}
	fclose(fptr);

	return 0;
}
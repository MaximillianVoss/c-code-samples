#pragma once
#include "stdafx.h"

typedef struct
{
	int n1;
	int n2;
	int n3;
} ThreeNum;

// Path-based helpers keep file I/O examples testable and avoid hard-coded
// absolute paths like C:\program.txt.
static int FileWriteText(const char* path, const char* text)
{
	FILE* fptr = fopen(path, "w");
	if (fptr == NULL)
	{
		return 1;
	}

	if (fprintf(fptr, "%s", text == NULL ? "" : text) < 0)
	{
		fclose(fptr);
		return 1;
	}

	fclose(fptr);
	return 0;
}

static int FileWriteInt(const char* path, int value)
{
	FILE* fptr = fopen(path, "w");
	if (fptr == NULL)
	{
		return 1;
	}

	if (fprintf(fptr, "%d", value) < 0)
	{
		fclose(fptr);
		return 1;
	}

	fclose(fptr);
	return 0;
}

static int FileReadInt(const char* path, int* value)
{
	if (value == NULL)
	{
		return 1;
	}

	FILE* fptr = fopen(path, "r");
	if (fptr == NULL)
	{
		return 1;
	}

	if (fscanf(fptr, "%d", value) != 1)
	{
		fclose(fptr);
		return 1;
	}

	fclose(fptr);
	return 0;
}

static int FileWriteThreeNumArray(const char* path, const ThreeNum* values, size_t length)
{
	if (values == NULL && length > 0)
	{
		return 1;
	}

	FILE* fptr = fopen(path, "wb");
	if (fptr == NULL)
	{
		return 1;
	}

	size_t written = fwrite(values, sizeof(ThreeNum), length, fptr);
	fclose(fptr);
	return written == length ? 0 : 1;
}

static int FileReadThreeNumArray(const char* path, ThreeNum* values, size_t length)
{
	if (values == NULL && length > 0)
	{
		return 1;
	}

	FILE* fptr = fopen(path, "rb");
	if (fptr == NULL)
	{
		return 1;
	}

	size_t read = fread(values, sizeof(ThreeNum), length, fptr);
	fclose(fptr);
	return read == length ? 0 : 1;
}

static void Read(void)
{
	FileWriteText("newprogram.txt", "Code samples C\n");
}

static int ReadEx(void)
{
	int num;

	printf("Enter num: ");
	if (scanf("%d", &num) != 1)
	{
		return 1;
	}

	return FileWriteInt("program.txt", num);
}

static int WriteEx(void)
{
	int num;
	if (FileReadInt("program.txt", &num) != 0)
	{
		printf("Error! opening file");
		return 1;
	}

	printf("Value of n=%d", num);
	return 0;
}

static int WriteBin(void)
{
	ThreeNum values[4];
	for (int n = 1; n < 5; ++n)
	{
		values[n - 1].n1 = n;
		values[n - 1].n2 = 5 * n;
		values[n - 1].n3 = 5 * n + 1;
	}

	return FileWriteThreeNumArray("program.bin", values, 4);
}

static int ReadBin(void)
{
	ThreeNum values[4];
	if (FileReadThreeNumArray("program.bin", values, 4) != 0)
	{
		printf("Error! opening file");
		return 1;
	}

	for (int n = 0; n < 4; ++n)
	{
		printf("n1: %d\tn2: %d\tn3: %d\n", values[n].n1, values[n].n2, values[n].n3);
	}

	return 0;
}

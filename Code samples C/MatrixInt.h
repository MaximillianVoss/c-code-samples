#pragma once
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma region Матрица

typedef struct
{
	size_t length;
	int* items;
} ArrayInt;

ArrayInt ArrayIntInit(size_t length)
{
	ArrayInt array;
	array.items = (int*)calloc(length, sizeof(int));
	array.length = length;
	return array;
}

void ArrayIntDelete(ArrayInt* array)
{
	if (array != NULL)
	{
		free(array->items);
		array->items = NULL;
		array->length = 0;
	}
}

void ArrayIntAdd(ArrayInt* array, int value)
{
	if (array == NULL)
	{
		return;
	}

	int* result = (int*)realloc(array->items, (array->length + 1) * sizeof(int));
	if (result == NULL)
	{
		return;
	}

	array->items = result;
	array->items[array->length] = value;
	array->length++;
}

int* ArrayIntGet(ArrayInt* array, size_t index)
{
	if (array != NULL && index < array->length)
	{
		return &array->items[index];
	}
	return NULL;
}

void ArrayIntSet(ArrayInt* array, size_t index, int value)
{
	if (array != NULL && index < array->length)
	{
		array->items[index] = value;
	}
}

typedef struct
{
	ArrayInt* rows;
	size_t rowsCount;
	size_t columnsCount;
} MatrixInt;

MatrixInt MatrixIntInit(size_t rowsCount, size_t columnsCount)
{
	MatrixInt matrix;
	matrix.rows = (ArrayInt*)calloc(rowsCount, sizeof(ArrayInt));
	matrix.rowsCount = rowsCount;
	matrix.columnsCount = columnsCount;

	for (size_t i = 0; i < rowsCount; i++)
	{
		matrix.rows[i] = ArrayIntInit(columnsCount);
	}

	return matrix;
}

void MatrixIntDelete(MatrixInt* matrix)
{
	if (matrix != NULL)
	{
		for (size_t i = 0; i < matrix->rowsCount; i++)
		{
			ArrayIntDelete(&matrix->rows[i]);
		}
		free(matrix->rows);
		matrix->rows = NULL;
		matrix->rowsCount = 0;
		matrix->columnsCount = 0;
	}
}

int* MatrixIntGet(MatrixInt* matrix, size_t rowIndex, size_t columnIndex)
{
	if (matrix != NULL && rowIndex < matrix->rowsCount && columnIndex < matrix->columnsCount)
	{
		return ArrayIntGet(&matrix->rows[rowIndex], columnIndex);
	}
	return NULL;
}

void MatrixIntSet(MatrixInt* matrix, size_t rowIndex, size_t columnIndex, int value)
{
	if (matrix != NULL && rowIndex < matrix->rowsCount && columnIndex < matrix->columnsCount)
	{
		ArrayIntSet(&matrix->rows[rowIndex], columnIndex, value);
	}
}

void MatrixIntInitRandom(MatrixInt* matrix, int minValue, int maxValue)
{
	if (matrix == NULL || minValue > maxValue)
	{
		return;
	}

	int range = maxValue - minValue + 1;
	for (size_t i = 0; i < matrix->rowsCount; i++)
	{
		for (size_t j = 0; j < matrix->columnsCount; j++)
		{
			MatrixIntSet(matrix, i, j, rand() % range + minValue);
		}
	}
}

MatrixInt MatrixIntTransponse(MatrixInt* matrix)
{
	MatrixInt result = MatrixIntInit(0, 0);
	if (matrix != NULL)
	{
		result = MatrixIntInit(matrix->columnsCount, matrix->rowsCount);
		for (size_t i = 0; i < matrix->rowsCount; i++)
		{
			for (size_t j = 0; j < matrix->columnsCount; j++)
			{
				int* value = MatrixIntGet(matrix, i, j);
				if (value != NULL)
				{
					MatrixIntSet(&result, j, i, *value);
				}
			}
		}
	}
	return result;
}

MatrixInt MatrixIntMultiply(MatrixInt a, MatrixInt b)
{
	MatrixInt result = MatrixIntInit(0, 0);
	if (a.columnsCount == b.rowsCount)
	{
		result = MatrixIntInit(a.rowsCount, b.columnsCount);
		for (size_t i = 0; i < result.rowsCount; i++)
		{
			for (size_t j = 0; j < result.columnsCount; j++)
			{
				int sum = 0;
				for (size_t k = 0; k < a.columnsCount; k++)
				{
					int* itemA = MatrixIntGet(&a, i, k);
					int* itemB = MatrixIntGet(&b, k, j);
					if (itemA != NULL && itemB != NULL)
					{
						sum += (*itemA) * (*itemB);
					}
				}
				MatrixIntSet(&result, i, j, sum);
			}
		}
	}
	return result;
}

void MatrixIntPrint(MatrixInt* matrix)
{
	if (matrix != NULL)
	{
		for (size_t i = 0; i < matrix->rowsCount; i++)
		{
			for (size_t j = 0; j < matrix->columnsCount; j++)
			{
				int* value = MatrixIntGet(matrix, i, j);
				printf("%9i", value == NULL ? 0 : *value);
			}
			printf("\n");
		}
	}
}

void MatrinxIntPrint(MatrixInt* matrix)
{
	MatrixIntPrint(matrix);
}

#pragma endregion

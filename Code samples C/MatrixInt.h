#pragma once
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma region Matrix

typedef struct
{
	size_t length;
	int* items;
} ArrayInt;

static ArrayInt ArrayIntInit(size_t length)
{
	ArrayInt array;
	array.items = (int*)calloc(length, sizeof(int));
	array.length = array.items == NULL && length > 0 ? 0 : length;
	return array;
}

static void ArrayIntDelete(ArrayInt* array)
{
	if (array != NULL)
	{
		free(array->items);
		array->items = NULL;
		array->length = 0;
	}
}

// Appends value if memory can be expanded. On allocation failure the array is
// left unchanged.
static void ArrayIntAdd(ArrayInt* array, int value)
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

static int* ArrayIntGet(ArrayInt* array, size_t index)
{
	if (array != NULL && array->items != NULL && index < array->length)
	{
		return &array->items[index];
	}
	return NULL;
}

static const int* ArrayIntGetConst(const ArrayInt* array, size_t index)
{
	if (array != NULL && array->items != NULL && index < array->length)
	{
		return &array->items[index];
	}
	return NULL;
}

static void ArrayIntSet(ArrayInt* array, size_t index, int value)
{
	int* item = ArrayIntGet(array, index);
	if (item != NULL)
	{
		*item = value;
	}
}

typedef struct
{
	ArrayInt* rows;
	size_t rowsCount;
	size_t columnsCount;
} MatrixInt;

static MatrixInt MatrixIntEmpty(void)
{
	MatrixInt matrix;
	matrix.rows = NULL;
	matrix.rowsCount = 0;
	matrix.columnsCount = 0;
	return matrix;
}

// Creates a rowsCount x columnsCount matrix initialized with zeroes.
// If any allocation fails, returns an empty 0x0 matrix.
static MatrixInt MatrixIntInit(size_t rowsCount, size_t columnsCount)
{
	MatrixInt matrix = MatrixIntEmpty();
	if (rowsCount == 0 || columnsCount == 0)
	{
		return matrix;
	}

	matrix.rows = (ArrayInt*)calloc(rowsCount, sizeof(ArrayInt));
	if (matrix.rows == NULL)
	{
		return MatrixIntEmpty();
	}

	matrix.rowsCount = rowsCount;
	matrix.columnsCount = columnsCount;
	for (size_t i = 0; i < rowsCount; i++)
	{
		matrix.rows[i] = ArrayIntInit(columnsCount);
		if (matrix.rows[i].length != columnsCount)
		{
			for (size_t j = 0; j <= i; j++)
			{
				ArrayIntDelete(&matrix.rows[j]);
			}
			free(matrix.rows);
			return MatrixIntEmpty();
		}
	}

	return matrix;
}

static void MatrixIntDelete(MatrixInt* matrix)
{
	if (matrix != NULL)
	{
		for (size_t i = 0; i < matrix->rowsCount; i++)
		{
			ArrayIntDelete(&matrix->rows[i]);
		}
		free(matrix->rows);
		*matrix = MatrixIntEmpty();
	}
}

static bool MatrixIntIsEmpty(const MatrixInt* matrix)
{
	return matrix == NULL || matrix->rows == NULL || matrix->rowsCount == 0 || matrix->columnsCount == 0;
}

static int* MatrixIntGet(MatrixInt* matrix, size_t rowIndex, size_t columnIndex)
{
	if (!MatrixIntIsEmpty(matrix) && rowIndex < matrix->rowsCount && columnIndex < matrix->columnsCount)
	{
		return ArrayIntGet(&matrix->rows[rowIndex], columnIndex);
	}
	return NULL;
}

static const int* MatrixIntGetConst(const MatrixInt* matrix, size_t rowIndex, size_t columnIndex)
{
	if (!MatrixIntIsEmpty(matrix) && rowIndex < matrix->rowsCount && columnIndex < matrix->columnsCount)
	{
		return ArrayIntGetConst(&matrix->rows[rowIndex], columnIndex);
	}
	return NULL;
}

static void MatrixIntSet(MatrixInt* matrix, size_t rowIndex, size_t columnIndex, int value)
{
	int* item = MatrixIntGet(matrix, rowIndex, columnIndex);
	if (item != NULL)
	{
		*item = value;
	}
}

static void MatrixIntInitRandom(MatrixInt* matrix, int minValue, int maxValue)
{
	if (MatrixIntIsEmpty(matrix) || minValue > maxValue)
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

static MatrixInt MatrixIntTransponse(const MatrixInt* matrix)
{
	MatrixInt result = MatrixIntEmpty();
	if (!MatrixIntIsEmpty(matrix))
	{
		result = MatrixIntInit(matrix->columnsCount, matrix->rowsCount);
		for (size_t i = 0; i < matrix->rowsCount; i++)
		{
			for (size_t j = 0; j < matrix->columnsCount; j++)
			{
				const int* value = MatrixIntGetConst(matrix, i, j);
				if (value != NULL)
				{
					MatrixIntSet(&result, j, i, *value);
				}
			}
		}
	}
	return result;
}

static MatrixInt MatrixIntMultiply(const MatrixInt* a, const MatrixInt* b)
{
	MatrixInt result = MatrixIntEmpty();
	if (MatrixIntIsEmpty(a) || MatrixIntIsEmpty(b) || a->columnsCount != b->rowsCount)
	{
		return result;
	}

	result = MatrixIntInit(a->rowsCount, b->columnsCount);
	for (size_t i = 0; i < result.rowsCount; i++)
	{
		for (size_t j = 0; j < result.columnsCount; j++)
		{
			int sum = 0;
			for (size_t k = 0; k < a->columnsCount; k++)
			{
				const int* itemA = MatrixIntGetConst(a, i, k);
				const int* itemB = MatrixIntGetConst(b, k, j);
				if (itemA != NULL && itemB != NULL)
				{
					sum += (*itemA) * (*itemB);
				}
			}
			MatrixIntSet(&result, i, j, sum);
		}
	}

	return result;
}

static void MatrixIntPrint(const MatrixInt* matrix)
{
	if (!MatrixIntIsEmpty(matrix))
	{
		for (size_t i = 0; i < matrix->rowsCount; i++)
		{
			for (size_t j = 0; j < matrix->columnsCount; j++)
			{
				const int* value = MatrixIntGetConst(matrix, i, j);
				printf("%9i", value == NULL ? 0 : *value);
			}
			printf("\n");
		}
	}
}

// Kept for compatibility with the original misspelled sample function name.
static void MatrinxIntPrint(const MatrixInt* matrix)
{
	MatrixIntPrint(matrix);
}

#pragma endregion

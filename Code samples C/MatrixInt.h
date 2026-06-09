#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#pragma region Матрица

typedef struct
{
	size_t length;
	int* items;
} ArrayInt;

ArrayInt ArrayIntInit(size_t length) {
	ArrayInt array;
	array.items = (int*)calloc(length, sizeof(int));
	array.length = length;
	return array;
}
void ArrayIntDelete(ArrayInt* array) {
	if (array != NULL)
		free(array->items);
}

void ArrayIntAdd(ArrayInt* array, int value) {
	if (array != NULL) {
		array->length++;
		array->items = (int*)realloc(array->items, array->length * sizeof(int));
		if (array->items != NULL)
			array->items[array->length - 1] = value;
	}
}

int* ArrayIntGet(ArrayInt* array, size_t index) {
	if (array != NULL && index < array->length)
		return &array->items[index];
	return NULL;
}

void ArrayIntSet(ArrayInt* array, int index, int value) {
	if (array != NULL && index >= 0 && index < array->length)
		array->items[index] = value;
}

typedef struct
{
	ArrayInt* rows;
	size_t rowsCount;
	size_t columnsCount;
} MatrixInt;

MatrixInt MatrixIntInit(size_t rowsCount, size_t columnsCount) {
	MatrixInt matrix;
	matrix.rows = (ArrayInt*)calloc(rowsCount, sizeof(ArrayInt));
	matrix.rowsCount = rowsCount;
	matrix.columnsCount = columnsCount;
	for (int i = 0; i < rowsCount; i++) {
		matrix.rows[i] = ArrayIntInit(columnsCount);
	}
	return matrix;
}

void MatrixIntDelete(MatrixInt* matrix) {
	if (matrix != NULL) {
		for (int i = 0; i < matrix->rowsCount; i++)
			ArrayIntDelete(&matrix->rows[i]);
	}
}

int* MatrixIntGet(MatrixInt* matrix, size_t rowIndex, size_t columnIndex) {
	if (matrix != NULL && rowIndex < matrix->rowsCount && columnIndex < matrix->columnsCount)
		return ArrayIntGet(&matrix->rows[rowIndex], columnIndex);
	return NULL;
}

void MatrixIntSet(MatrixInt* matrix, size_t rowIndex, size_t columnIndex, int value) {
	if (matrix != NULL && rowIndex < matrix->rowsCount && columnIndex < matrix->columnsCount)
		ArrayIntSet(&matrix->rows[rowIndex], columnIndex, value);
}

void MatrixIntInitRandom(MatrixInt* matrix, int minValue, int maxValue) {
	if (matrix) {
		for (int i = 0; i < matrix->rowsCount; i++)
		{
			for (int j = 0; j < matrix->columnsCount; j++) {
				MatrixIntSet(matrix, i, j, rand() % maxValue + minValue);
			}
		}
	}
}

MatrixInt MatrixIntTransponse(MatrixInt* matrix) {
	MatrixInt result = MatrixIntInit(0, 0);
	if (matrix != NULL) {
		MatrixIntDelete(&result);
		result = MatrixIntInit(matrix->columnsCount, matrix->rowsCount);
		for (size_t i = 0; i < matrix->rowsCount; i++)
		{
			for (size_t j = 0; j < matrix->columnsCount; j++)
			{
				MatrixIntSet(&result, j, i, *MatrixIntGet(matrix, i, j));
			}
		}
	}
	return result;
}

MatrixInt MatrixIntMultiply(MatrixInt a, MatrixInt b) {
	MatrixInt result = MatrixIntInit(0, 0);
	if (a.columnsCount == b.rowsCount) {
		MatrixIntDelete(&result);
		result = MatrixIntInit(a.rowsCount, b.columnsCount);
		for (int i = 0; i < result.rowsCount; i++) {
			for (int j = 0; j < result.columnsCount; j++) {
				int sum = 0;
				for (int k = 0; k < a.columnsCount; k++) {
					int itemA = (*MatrixIntGet(&a, i, k));
					int itemB = (*MatrixIntGet(&b, k, j));
					int item = (itemA * itemB);
					sum = sum + item;
				}
				MatrixIntSet(&result, i, j, sum);
			}
		}
	}
	return result;
}

void MatrinxIntPrint(MatrixInt* matrix) {
	if (matrix != NULL) {
		for (int i = 0; i < matrix->rowsCount; i++)
		{
			for (int j = 0; j < matrix->columnsCount; j++) {
				printf("%9i", *MatrixIntGet(matrix, i, j));
			}
			printf("\n");
		}
	}
}
#pragma endregion

#include "array_exercises.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

c_array_status c_array_resize(int **items, size_t *size, ptrdiff_t delta)
{
    size_t new_size;

    if (items == NULL || size == NULL || (*items == NULL && *size != 0U)) {
        return C_ARRAY_INVALID_ARGUMENT;
    }

    if (delta < 0) {
        const size_t shrink = (size_t)(-(delta + 1)) + 1U;
        if (shrink > *size) {
            return C_ARRAY_INVALID_ARGUMENT;
        }
        new_size = *size - shrink;
    } else {
        const size_t growth = (size_t)delta;
        if (*size > SIZE_MAX - growth) {
            return C_ARRAY_SIZE_OVERFLOW;
        }
        new_size = *size + growth;
    }

    if (new_size == 0U) {
        free(*items);
        *items = NULL;
        *size = 0U;
        return C_ARRAY_OK;
    }

    if (new_size > SIZE_MAX / sizeof(**items)) {
        return C_ARRAY_SIZE_OVERFLOW;
    }

    {
        const size_t old_size = *size;
        int *resized = (int *)realloc(*items, new_size * sizeof(*resized));
        if (resized == NULL) {
            return C_ARRAY_ALLOCATION_FAILED;
        }

        if (new_size > old_size) {
            memset(resized + old_size, 0, (new_size - old_size) * sizeof(*resized));
        }

        *items = resized;
        *size = new_size;
    }

    return C_ARRAY_OK;
}
c_array_status c_matrix_sort_rows(int *items, size_t rows, size_t columns)
{
    size_t row;

    if (rows == 0U || columns == 0U) {
        return C_ARRAY_OK;
    }
    if (items == NULL) {
        return C_ARRAY_INVALID_ARGUMENT;
    }
    if (rows > SIZE_MAX / columns) {
        return C_ARRAY_SIZE_OVERFLOW;
    }

    for (row = 0U; row < rows; ++row) {
        size_t current;
        int *values = items + row * columns;

        for (current = 1U; current < columns; ++current) {
            const int value = values[current];
            size_t position = current;

            while (position > 0U && values[position - 1U] > value) {
                values[position] = values[position - 1U];
                --position;
            }
            values[position] = value;
        }
    }

    return C_ARRAY_OK;
}

#ifndef C_ARRAY_EXERCISES_H
#define C_ARRAY_EXERCISES_H

#include <stddef.h>

typedef enum c_array_status {
    C_ARRAY_OK = 0,
    C_ARRAY_INVALID_ARGUMENT,
    C_ARRAY_SIZE_OVERFLOW,
    C_ARRAY_ALLOCATION_FAILED
} c_array_status;

c_array_status c_array_resize(int **items, size_t *size, ptrdiff_t delta);
c_array_status c_matrix_sort_rows(int *items, size_t rows, size_t columns);

#endif

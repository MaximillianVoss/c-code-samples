#include "array_exercises.h"

#include <stdio.h>
#include <stdlib.h>

static int failures = 0;

#define CHECK(condition)                                                        \
    do {                                                                        \
        if (!(condition)) {                                                     \
            fprintf(stderr, "check failed at line %d: %s\n", __LINE__, #condition); \
            ++failures;                                                        \
        }                                                                       \
    } while (0)

static void test_resize_grows_and_zero_initializes(void)
{
    size_t size = 2U;
    int *items = (int *)malloc(size * sizeof(*items));

    CHECK(items != NULL);
    if (items == NULL) {
        return;
    }
    items[0] = 7;
    items[1] = -3;

    CHECK(c_array_resize(&items, &size, 2) == C_ARRAY_OK);
    CHECK(size == 4U);
    CHECK(items[0] == 7 && items[1] == -3);
    CHECK(items[2] == 0 && items[3] == 0);

    free(items);
}

static void test_resize_shrinks_and_rejects_underflow(void)
{
    size_t size = 4U;
    int *items = (int *)malloc(size * sizeof(*items));
    int *before;

    CHECK(items != NULL);
    if (items == NULL) {
        return;
    }
    items[0] = 4;
    items[1] = 3;
    items[2] = 2;
    items[3] = 1;

    CHECK(c_array_resize(&items, &size, -2) == C_ARRAY_OK);
    CHECK(size == 2U);
    CHECK(items[0] == 4 && items[1] == 3);

    before = items;
    CHECK(c_array_resize(&items, &size, -3) == C_ARRAY_INVALID_ARGUMENT);
    CHECK(items == before && size == 2U);

    CHECK(c_array_resize(&items, &size, -2) == C_ARRAY_OK);
    CHECK(items == NULL && size == 0U);
}

static void test_resize_validates_state(void)
{
    size_t size = 1U;
    int *items = NULL;

    CHECK(c_array_resize(NULL, &size, 1) == C_ARRAY_INVALID_ARGUMENT);
    CHECK(c_array_resize(&items, NULL, 1) == C_ARRAY_INVALID_ARGUMENT);
    CHECK(c_array_resize(&items, &size, 1) == C_ARRAY_INVALID_ARGUMENT);
}

static void test_matrix_rows_are_sorted_independently(void)
{
    int values[] = {9, 1, 4, -2, 8, 8};
    const int expected[] = {1, 4, 9, -2, 8, 8};
    size_t index;

    CHECK(c_matrix_sort_rows(values, 2U, 3U) == C_ARRAY_OK);
    for (index = 0U; index < sizeof(values) / sizeof(values[0]); ++index) {
        CHECK(values[index] == expected[index]);
    }

    CHECK(c_matrix_sort_rows(NULL, 0U, 3U) == C_ARRAY_OK);
    CHECK(c_matrix_sort_rows(NULL, 2U, 3U) == C_ARRAY_INVALID_ARGUMENT);
}

int main(void)
{
    test_resize_grows_and_zero_initializes();
    test_resize_shrinks_and_rejects_underflow();
    test_resize_validates_state();
    test_matrix_rows_are_sorted_independently();

    if (failures != 0) {
        fprintf(stderr, "%d array exercise check(s) failed\n", failures);
        return EXIT_FAILURE;
    }

    puts("All C array exercise tests passed.");
    return EXIT_SUCCESS;
}

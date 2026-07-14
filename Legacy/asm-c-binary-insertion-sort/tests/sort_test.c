#include "vehicle.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef SORT_IMPLEMENTATION
#define SORT_IMPLEMENTATION insertionSort
#endif

static int is_sorted(const Vehicle *vehicles, size_t count)
{
    for (size_t index = 1; index < count; ++index) {
        if (vehicles[index - 1].hash > vehicles[index].hash) {
            return 0;
        }
    }
    return 1;
}

static int run_case(const Vehicle *input, size_t count)
{
    Vehicle actual[8] = {0};
    Vehicle expected[8] = {0};

    if (count > sizeof(actual) / sizeof(actual[0])) {
        return 0;
    }

    memcpy(actual, input, count * sizeof(*input));
    memcpy(expected, input, count * sizeof(*input));

    SORT_IMPLEMENTATION(actual, (int)count);
    insertion_sort_reference(expected, count);

    return is_sorted(actual, count)
        && memcmp(actual, expected, count * sizeof(*actual)) == 0;
}

int main(void)
{
    const Vehicle unsorted[] = {
        {.hash = 9, .type = 1},
        {.hash = -3, .type = 2},
        {.hash = 14, .type = 3},
        {.hash = 0, .type = 4},
        {.hash = 7, .type = 5},
    };
    const Vehicle one[] = {{.hash = 5, .type = 1}};

    if (!run_case(unsorted, sizeof(unsorted) / sizeof(unsorted[0]))
        || !run_case(one, sizeof(one) / sizeof(one[0]))) {
        fputs("binary insertion sort test failed\n", stderr);
        return EXIT_FAILURE;
    }

    puts("binary insertion sort tests passed");
    return EXIT_SUCCESS;
}

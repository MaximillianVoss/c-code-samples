#ifndef ASM_C_BINARY_INSERTION_SORT_VEHICLE_H
#define ASM_C_BINARY_INSERTION_SORT_VEHICLE_H

#include <stddef.h>

typedef struct Vehicle {
    int hash;
    int type;
    int carry_capacity;
    int passenger_places;
    int max_speed;
    int tank_capacity;
    double fuel_consumption;
} Vehicle;

/* The preserved assembly advances through records in fixed 32-byte steps. */
_Static_assert(sizeof(Vehicle) == 32, "Vehicle layout no longer matches the assembly ABI");

int binarySearch(Vehicle *vehicles, Vehicle item, int low, int high);
void insertionSort(Vehicle *vehicles, int count);

void insertion_sort_reference(Vehicle *vehicles, size_t count);

#endif

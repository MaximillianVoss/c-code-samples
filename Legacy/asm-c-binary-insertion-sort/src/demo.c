#include "vehicle.h"

#include <stdio.h>
#include <stdlib.h>

enum { VEHICLE_COUNT = 10 };

static int estimated_range(const Vehicle *vehicle)
{
    return (int)((double)vehicle->tank_capacity / vehicle->fuel_consumption);
}

static void initialize_vehicles(Vehicle vehicles[VEHICLE_COUNT])
{
    for (size_t index = 0; index < VEHICLE_COUNT; ++index) {
        vehicles[index] = (Vehicle) {
            .type = 1 + rand() % 3,
            .tank_capacity = 1 + rand() % 100,
            .fuel_consumption = 1 + rand() % 10,
        };
        vehicles[index].hash = estimated_range(&vehicles[index]);
    }
}

static void print_vehicles(const Vehicle *vehicles, size_t count)
{
    for (size_t index = 0; index < count; ++index) {
        printf("range:%5d type:%5d tank:%5d consumption:%5.2f\n",
               vehicles[index].hash,
               vehicles[index].type,
               vehicles[index].tank_capacity,
               vehicles[index].fuel_consumption);
    }
}

int main(void)
{
    Vehicle vehicles[VEHICLE_COUNT];

    srand(42u);
    initialize_vehicles(vehicles);

    puts("Before sort:");
    print_vehicles(vehicles, VEHICLE_COUNT);
    insertionSort(vehicles, VEHICLE_COUNT);
    puts("After sort:");
    print_vehicles(vehicles, VEHICLE_COUNT);

    return EXIT_SUCCESS;
}

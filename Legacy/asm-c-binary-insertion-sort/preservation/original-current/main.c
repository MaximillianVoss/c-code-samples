#pragma region Includes
#include <stdio.h>
// для функций rand() и srand()
#include <stdlib.h>
// для функции time()
#include <time.h>
#include <string.h>
#include "data.h"
#pragma endregion

#pragma region Structure
struct Vehicle
{
    /// <summary>
    /// HASH function result (distance)
    /// </summary>
    int hash;
    int type;
    int carryCapacity;
    int passengersPlaces;
    int maxSpeed;
    int tankCapacity;
    double fuelConsumption;
};
#pragma endregion

#pragma region ASM sort
int binarySearch(struct Vehicle *a, struct Vehicle item, int low, int high);
void insertionSort(struct Vehicle *a, int n);
#pragma endregion

#pragma region C sort
int binarySearchC(struct Vehicle *a, struct Vehicle item, int low, int high)
{
    if (high <= low)
        return (item.hash > a[low].hash) ? (low + 1) : low;
    int mid = (low + high) / 2;
    if (item.hash == a[mid].hash)
        return mid + 1;
    if (item.hash > a[mid].hash)
        return binarySearchC(a, item,
                             mid + 1, high);
    return binarySearchC(a, item, low,
                         mid - 1);
}
void insertionSortC(struct Vehicle *a, int n)
{
    int i, loc, j, k;
    struct Vehicle selected;
    for (i = 1; i < n; ++i)
    {
        j = i - 1;
        selected = a[i];
        loc = binarySearchC(a, selected, 0, j);
        while (j >= loc)
        {
            a[j + 1] = a[j];
            j--;
        }
        a[j + 1] = selected;
    }
}
#pragma endregion

#pragma region Methods
void GetDistance(struct Vehicle *vehicle)
{
    vehicle->hash = ((double)vehicle->tankCapacity) / vehicle->fuelConsumption;
}
void Print(struct Vehicle *vehicles, int length)
{
    for (int i = 0; i < length; i++)
        printf("hash:%5i type:%5i tank:%5i consumption:%.3lf\n",
               vehicles[i].hash, vehicles[i].type, vehicles[i].tankCapacity, vehicles[i].fuelConsumption);
}
void TaskCars()
{
    int length = 10;
    struct Vehicle *vehicles = (struct Vehicle *)malloc(sizeof(struct Vehicle) * length);
    for (int i = 0; i < length; i++)
    {
        vehicles[i].type = 1 + rand() % 3;
        vehicles[i].fuelConsumption = 1 + rand() % 10;
        vehicles[i].tankCapacity = 1 + rand() % 100;
        GetDistance(&vehicles[i]);
    }
    printf("sizeof car:%li bytes\n", sizeof(struct Vehicle));
    printf("Before sort:\n");
    Print(vehicles, length);
    insertionSort(vehicles, length);
    printf("After sort:\n");
    Print(vehicles, length);
}
#pragma endregion

#pragma region Main
int main(int argc, char *argv[])
{
    TaskCars();
    return 0;
}
#pragma endregion

#pragma region Bash commands
//make
//./task
#pragma endregion
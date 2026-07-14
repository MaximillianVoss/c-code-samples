//------------------------------------------------------------------------------
// main.cpp - содержит главную функцию,
// обеспечивающую простое тестирование
//------------------------------------------------------------------------------

#include <stdio.h>
// для функций rand() и srand()
#include <stdlib.h>
// для функции time()
#include <time.h>
#include <string.h>
#include "data.h"

// #include "input.c"
// #include "inrnd.c"
// #include "perimeter.c"
// #include "output.c"

void errMessage1()
{
    printf("incorrect command line!\n"
           "  Waited:\n"
           "     command -f infile outfile01 outfile02\n"
           "  Or:\n"
           "     command -n number outfile01 outfile02\n");
}

void errMessage2()
{
    printf("incorrect qualifier value!\n"
           "  Waited:\n"
           "     command -f infile outfile01 outfile02\n"
           "  Or:\n"
           "     command -n number outfile01 outfile02\n");
}
//------------------------------------------------------------------------------

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

int binarySearch(struct Vehicle *a, struct Vehicle item,
                 int low, int high);
// {
//     if (high <= low)
//         return (item.hash > a[low].hash) ? (low + 1) : low;

//     int mid = (low + high) / 2;

//     if (item.hash == a[mid].hash)
//         return mid + 1;

//     if (item.hash > a[mid].hash)
//         return binarySearch(a, item,
//                             mid + 1, high);
//     return binarySearch(a, item, low,
//                         mid - 1);
// }

void insertionSort(struct Vehicle *a, int n);
// {
//     int i, loc, j, k;
//     struct Vehicle selected;
//     for (i = 1; i < n; ++i)
//     {
//         j = i - 1;
//         selected = a[i];
//         // find location where selected should be inseretd
//         loc = binarySearch(a, selected, 0, j);
//         // Move all elements after location to create space
//         while (j >= loc)
//         {
//             a[j + 1] = a[j];
//             j--;
//         }
//         a[j + 1] = selected;
//     }
// }

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
    printf("before\n");
    Print(vehicles, length);
    insertionSort(vehicles, length);
    printf("after\n");
    Print(vehicles, length);
}

//make
//./task
int main(int argc, char *argv[])
{
    TaskCars();
#pragma region EXAMPLE
    // Массив используемый для хранения данных
    //unsigned int cont[maxSize / intSize];
    //int cont[maxSize / intSize];
    // unsigned char cont[maxSize];
    // // Количество элементов в массиве
    // int len = 0;

    // printf("intSize = %d\n", intSize);
    // printf("rectSize = %d\n", rectSize);
    // printf("trianSize = %d\n", trianSize);
    // printf("shapeSize = %d\n", shapeSize);
    // printf("maxSize = %d\n", maxSize);
    // printf("RECTANGLE = %d\n", RECTANGLE);
    // printf("TRIANGLE = %d\n", TRIANGLE);
    // printf("Size of container = %d\n", sizeof(cont));

    // if (argc != 5)
    // {
    //     errMessage1();
    //     return 1;
    // }

    // printf("Start\n");
    // //InitContainer(&cont);

    // if (!strcmp(argv[1], "-f"))
    // {
    //     FILE *ifst = fopen(argv[2], "r");
    //     InContainer(cont, &len, ifst);
    // }
    // else if (!strcmp(argv[1], "-n"))
    // {
    //     int size = atoi(argv[2]);
    //     if ((size < 1) || (size > 10000))
    //     {
    //         printf("incorrect numer of figures = %d. Set 0 < number <= 10000\n",
    //                size);
    //         return 3;
    //     }
    //     // системные часы в качестве инициализатора
    //     srand((unsigned int)(time(0)));
    //     // Заполнение контейнера генератором случайных чисел
    //     InRndContainer(cont, &len, size);
    // }
    // else
    // {
    //     errMessage2();
    //     return 2;
    // }

    // // Вывод содержимого контейнера в файл
    // fprintf(stdout, "Filled container:\n");
    // OutContainer(cont, len, stdout);
    // FILE *ofst1 = fopen(argv[3], "w");
    // fprintf(ofst1, "Filled container:\n");
    // OutContainer(cont, len, ofst1);
    // fclose(ofst1);

    // // The 2nd part of task
    // FILE *ofst2 = fopen(argv[4], "w");
    // clock_t start = clock();
    // double sum = PerimeterSumContainer(cont, len);
    // clock_t end = clock();
    // double calcTime = ((double)(end - start)) / (CLOCKS_PER_SEC + 1.0);

    // fprintf(stdout, "Perimeter sum = %g\nCalculaton time = %g\n", sum, calcTime);
    // fprintf(ofst2, "Perimeter sum = %g\nCalculaton time = %g\n", sum, calcTime);
    // fclose(ofst2);

    // //ClearContainer(&c);
    // printf("Stop\n");

#pragma endregion
    return 0;
}

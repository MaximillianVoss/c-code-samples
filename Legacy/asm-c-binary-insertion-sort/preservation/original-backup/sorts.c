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

int binarySearch(struct Vehicle *a, struct Vehicle item,
                 int low, int high)
{
    if (high <= low)
        return (item.hash > a[low].hash) ? (low + 1) : low;

    int mid = (low + high) / 2;

    if (item.hash == a[mid].hash)
        return mid + 1;

    if (item.hash > a[mid].hash)
        return binarySearch(a, item,
                            mid + 1, high);
    return binarySearch(a, item, low,
                        mid - 1);
}

void insertionSort(struct Vehicle *a, int n)
{
    int i, loc, j, k;
    struct Vehicle selected;
    for (i = 1; i < n; ++i)
    {
        j = i - 1;
        selected = a[i];
        // find location where selected should be inseretd
        loc = binarySearch(a, selected, 0, j);
        // Move all elements after location to create space
        while (j >= loc)
        {
            a[j + 1] = a[j];
            j--;
        }
        a[j + 1] = selected;
    }
}

void TaskCars()
{
    // int length = 10;
    // struct Vehicle *vehicles = (struct Vehicle *)malloc(sizeof(struct Vehicle) * length);
    // for (int i = 0; i < length; i++)
    // {
    //     vehicles[i].type = 1 + rand() % 3;
    //     vehicles[i].fuelConsumption = 1 + rand() % 10;
    //     vehicles[i].tankCapacity = 1 + rand() % 100;
    //     GetDistance(&vehicles[i]);
    // }
    // printf("sizeof car:%i bytes\n", sizeof(struct Vehicle));
    // printf("before\n");
    // Print(vehicles, length);
    // insertionSort(vehicles, length);
    // printf("after\n");
    // Print(vehicles, length);

    struct Vehicle *vehicles;
    int length = 10;
    insertionSort(vehicles, length);
}

int main()
{
    TaskCars();
}
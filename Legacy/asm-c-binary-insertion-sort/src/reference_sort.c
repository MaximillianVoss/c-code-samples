#include "vehicle.h"

#include <string.h>

void insertion_sort_reference(Vehicle *vehicles, size_t count)
{
    for (size_t index = 1; index < count; ++index) {
        const Vehicle selected = vehicles[index];
        size_t low = 0;
        size_t high = index;

        while (low < high) {
            const size_t middle = low + (high - low) / 2;
            if (vehicles[middle].hash <= selected.hash) {
                low = middle + 1;
            } else {
                high = middle;
            }
        }

        memmove(&vehicles[low + 1],
                &vehicles[low],
                (index - low) * sizeof(*vehicles));
        vehicles[low] = selected;
    }
}

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"

void copy_area(PngImage *img, Point src_lu, Point src_rd, Point dest_lu)
{
    if (src_lu.x < 0 || src_lu.y < 0 || src_rd.x >= img->width || src_rd.y >= img->height ||
        dest_lu.x < 0 || dest_lu.y < 0 || src_lu.x > src_rd.x || src_lu.y > src_rd.y)
    {
        fprintf(stderr, "Error: Invalid copy coordinates\n");
        return;
    }

    int w = src_rd.x - src_lu.x + 1;
    int h = src_rd.y - src_lu.y + 1;

    if (dest_lu.x + w > img->width || dest_lu.y + h > img->height)
    {
        fprintf(stderr, "Error: Destination area is out of bounds\n");
        return;
    }

    // временный буфер
    png_bytep *buffer = malloc(h * sizeof(png_bytep));
    for (int y = 0; y < h; y++)
    {
        buffer[y] = malloc(w * 3);
        memcpy(buffer[y], &img->rows[src_lu.y + y][src_lu.x * 3], w * 3);
    }

    // вставка
    for (int y = 0; y < h; y++)
    {
        memcpy(&img->rows[dest_lu.y + y][dest_lu.x * 3], buffer[y], w * 3);
        free(buffer[y]);
    }
    free(buffer);
}

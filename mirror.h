#pragma once
#include "types.h"

void mirror_area(PngImage *img, Point lu, Point rd, char axis)
{
    if (lu.x < 0 || lu.y < 0 || rd.x >= img->width || rd.y >= img->height ||
        lu.x > rd.x || lu.y > rd.y)
    {
        fprintf(stderr, "Error: Invalid mirror area\n");
        return;
    }

    if (axis == 'x')
    {
        for (int y = lu.y; y <= rd.y; y++)
        {
            for (int x = 0; x <= (rd.x - lu.x) / 2; x++)
            {
                int xa = lu.x + x, xb = rd.x - x;
                png_bytep a = &img->rows[y][xa * 3];
                png_bytep b = &img->rows[y][xb * 3];
                for (int c = 0; c < 3; c++)
                {
                    png_byte tmp = a[c];
                    a[c] = b[c];
                    b[c] = tmp;
                }
            }
        }
    }
    else if (axis == 'y')
    {
        for (int y = 0; y <= (rd.y - lu.y) / 2; y++)
        {
            int ya = lu.y + y, yb = rd.y - y;
            for (int x = lu.x; x <= rd.x; x++)
            {
                png_bytep a = &img->rows[ya][x * 3];
                png_bytep b = &img->rows[yb][x * 3];
                for (int c = 0; c < 3; c++)
                {
                    png_byte tmp = a[c];
                    a[c] = b[c];
                    b[c] = tmp;
                }
            }
        }
    }
    else
    {
        fprintf(stderr, "Error: Unknown mirror axis\n");
    }
}

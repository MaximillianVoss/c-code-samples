#pragma once
#include <math.h>
#include <string.h>
#include "types.h"

void draw_circle(PngImage *img, Point center, int radius, int thickness, Color color, bool fill, Color fill_color)
{
    if (radius <= 0 || thickness <= 0)
        return;

    for (int y = 0; y < img->height; y++)
    {
        for (int x = 0; x < img->width; x++)
        {
            int dx = x - center.x;
            int dy = y - center.y;
            double dist = sqrt(dx * dx + dy * dy);
            png_bytep pixel = &img->rows[y][x * 3];

            if (fill && dist <= radius)
                memcpy(pixel, (png_byte[]){fill_color.r, fill_color.g, fill_color.b}, 3);

            if (dist >= radius - thickness / 2.0 && dist <= radius + thickness / 2.0)
                memcpy(pixel, (png_byte[]){color.r, color.g, color.b}, 3);
        }
    }
}

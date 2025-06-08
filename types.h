#pragma once
#include <png.h>

typedef struct
{
    int x, y;
} Point;
typedef struct
{
    int r, g, b;
} Color;

typedef struct
{
    int width;
    int height;
    png_byte color_type;
    png_byte bit_depth;
    png_bytep *rows;
} PngImage;

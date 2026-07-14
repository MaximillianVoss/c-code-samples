#ifndef PNG_IMAGE_TOOL_PNG_IO_H
#define PNG_IMAGE_TOOL_PNG_IO_H

#include "png_image_tool/image.h"

#include <stddef.h>

enum {
    PIT_PNG_COLOR_RGB = 2,
    PIT_PNG_COLOR_RGBA = 6,
    PIT_PNG_INTERLACE_NONE = 0,
    PIT_PNG_INTERLACE_ADAM7 = 1
};

typedef struct PitPngImage {
    PitImage image;
    int bit_depth;
    int color_type;
    int interlace_type;
    int compression_type;
    int filter_method;
} PitPngImage;

PitStatus pit_png_read(
    const char *path,
    PitPngImage *image,
    char *error,
    size_t error_size);

PitStatus pit_png_write(
    const char *path,
    const PitPngImage *image,
    char *error,
    size_t error_size);

void pit_png_image_destroy(PitPngImage *image);

#endif

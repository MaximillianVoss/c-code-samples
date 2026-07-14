#ifndef PNG_IMAGE_TOOL_OPERATIONS_H
#define PNG_IMAGE_TOOL_OPERATIONS_H

#include "png_image_tool/image.h"

#include <stdbool.h>
#include <stdint.h>

typedef struct PitPoint {
    int64_t x;
    int64_t y;
} PitPoint;

typedef struct PitRect {
    size_t left;
    size_t top;
    size_t right;
    size_t bottom;
} PitRect;

typedef struct PitColor {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} PitColor;

typedef enum PitMirrorAxis {
    PIT_MIRROR_LEFT_RIGHT = 0,
    PIT_MIRROR_TOP_BOTTOM
} PitMirrorAxis;

PitStatus pit_draw_circle(
    PitImage *image,
    PitPoint center,
    unsigned int radius,
    unsigned int thickness,
    PitColor line_color,
    bool fill,
    PitColor fill_color);

PitStatus pit_mirror_area(PitImage *image, PitRect area, PitMirrorAxis axis);
PitStatus pit_copy_area(PitImage *image, PitRect source, PitPoint destination);

#endif

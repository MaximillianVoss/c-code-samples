#ifndef PNG_IMAGE_TOOL_IMAGE_H
#define PNG_IMAGE_TOOL_IMAGE_H

#include <stddef.h>
#include <stdint.h>

typedef enum PitStatus {
    PIT_OK = 0,
    PIT_INVALID_ARGUMENT,
    PIT_OUT_OF_BOUNDS,
    PIT_ALLOCATION_FAILED,
    PIT_UNSUPPORTED_FORMAT,
    PIT_IO_ERROR,
    PIT_PNG_ERROR
} PitStatus;

typedef struct PitImage {
    size_t width;
    size_t height;
    size_t stride;
    unsigned int channels;
    uint8_t *pixels;
} PitImage;

const char *pit_status_message(PitStatus status);

/* The destination must be zero-initialized or previously destroyed. */
PitStatus pit_image_create(PitImage *image, size_t width, size_t height, unsigned int channels);
void pit_image_destroy(PitImage *image);

uint8_t *pit_image_pixel(PitImage *image, size_t x, size_t y);
const uint8_t *pit_image_const_pixel(const PitImage *image, size_t x, size_t y);

#endif

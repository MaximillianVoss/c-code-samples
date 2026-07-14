#include "png_image_tool/image.h"

#include <stdint.h>
#include <stdlib.h>

const char *pit_status_message(PitStatus status)
{
    switch (status) {
    case PIT_OK:
        return "success";
    case PIT_INVALID_ARGUMENT:
        return "invalid argument";
    case PIT_OUT_OF_BOUNDS:
        return "coordinates are outside the image";
    case PIT_ALLOCATION_FAILED:
        return "memory allocation failed";
    case PIT_UNSUPPORTED_FORMAT:
        return "unsupported image format";
    case PIT_IO_ERROR:
        return "file I/O failed";
    case PIT_PNG_ERROR:
        return "PNG processing failed";
    default:
        return "unknown error";
    }
}

PitStatus pit_image_create(PitImage *image, size_t width, size_t height, unsigned int channels)
{
    size_t stride;
    size_t byte_count;
    uint8_t *pixels;

    if (image == NULL || image->pixels != NULL || width == 0U || height == 0U
        || (channels != 3U && channels != 4U)) {
        return PIT_INVALID_ARGUMENT;
    }
    if (width > SIZE_MAX / channels) {
        return PIT_ALLOCATION_FAILED;
    }
    stride = width * channels;
    if (height > SIZE_MAX / stride) {
        return PIT_ALLOCATION_FAILED;
    }
    byte_count = stride * height;
    pixels = calloc(byte_count, 1U);
    if (pixels == NULL) {
        return PIT_ALLOCATION_FAILED;
    }

    image->width = width;
    image->height = height;
    image->stride = stride;
    image->channels = channels;
    image->pixels = pixels;
    return PIT_OK;
}

void pit_image_destroy(PitImage *image)
{
    if (image == NULL) {
        return;
    }
    free(image->pixels);
    image->pixels = NULL;
    image->width = 0U;
    image->height = 0U;
    image->stride = 0U;
    image->channels = 0U;
}

uint8_t *pit_image_pixel(PitImage *image, size_t x, size_t y)
{
    if (image == NULL || image->pixels == NULL || x >= image->width || y >= image->height) {
        return NULL;
    }
    return image->pixels + y * image->stride + x * image->channels;
}

const uint8_t *pit_image_const_pixel(const PitImage *image, size_t x, size_t y)
{
    if (image == NULL || image->pixels == NULL || x >= image->width || y >= image->height) {
        return NULL;
    }
    return image->pixels + y * image->stride + x * image->channels;
}

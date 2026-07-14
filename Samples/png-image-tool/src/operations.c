#include "png_image_tool/operations.h"

#include <stdlib.h>
#include <string.h>

static bool pit_image_is_valid(const PitImage *image)
{
    return image != NULL && image->pixels != NULL && image->width > 0U && image->height > 0U
        && (image->channels == 3U || image->channels == 4U)
        && image->width <= SIZE_MAX / image->channels
        && image->stride == image->width * image->channels;
}

static bool pit_rect_is_valid(const PitImage *image, PitRect area)
{
    return pit_image_is_valid(image) && area.left <= area.right && area.top <= area.bottom
        && area.right < image->width && area.bottom < image->height;
}

static void pit_set_rgb(uint8_t *pixel, PitColor color)
{
    pixel[0] = color.red;
    pixel[1] = color.green;
    pixel[2] = color.blue;
}

PitStatus pit_draw_circle(
    PitImage *image,
    PitPoint center,
    unsigned int radius,
    unsigned int thickness,
    PitColor line_color,
    bool fill,
    PitColor fill_color)
{
    double radius_value;
    double half_thickness;
    double inner_radius;
    double outer_radius;
    double radius_squared;
    double inner_squared;
    double outer_squared;
    size_t y;

    if (!pit_image_is_valid(image) || radius == 0U || thickness == 0U) {
        return PIT_INVALID_ARGUMENT;
    }

    radius_value = (double)radius;
    half_thickness = (double)thickness / 2.0;
    inner_radius = radius_value > half_thickness ? radius_value - half_thickness : 0.0;
    outer_radius = radius_value + half_thickness;
    radius_squared = radius_value * radius_value;
    inner_squared = inner_radius * inner_radius;
    outer_squared = outer_radius * outer_radius;

    for (y = 0U; y < image->height; ++y) {
        size_t x;
        for (x = 0U; x < image->width; ++x) {
            double dx = (double)x - (double)center.x;
            double dy = (double)y - (double)center.y;
            double distance_squared = dx * dx + dy * dy;
            uint8_t *pixel = pit_image_pixel(image, x, y);

            if (fill && distance_squared <= radius_squared) {
                pit_set_rgb(pixel, fill_color);
            }
            if (distance_squared >= inner_squared && distance_squared <= outer_squared) {
                pit_set_rgb(pixel, line_color);
            }
        }
    }
    return PIT_OK;
}

PitStatus pit_mirror_area(PitImage *image, PitRect area, PitMirrorAxis axis)
{
    size_t area_width;
    size_t area_height;

    if (!pit_rect_is_valid(image, area)) {
        return PIT_OUT_OF_BOUNDS;
    }
    if (axis != PIT_MIRROR_LEFT_RIGHT && axis != PIT_MIRROR_TOP_BOTTOM) {
        return PIT_INVALID_ARGUMENT;
    }

    area_width = area.right - area.left + 1U;
    area_height = area.bottom - area.top + 1U;

    if (axis == PIT_MIRROR_LEFT_RIGHT) {
        size_t y;
        for (y = area.top; y <= area.bottom; ++y) {
            size_t offset;
            for (offset = 0U; offset < area_width / 2U; ++offset) {
                uint8_t *left = pit_image_pixel(image, area.left + offset, y);
                uint8_t *right = pit_image_pixel(image, area.right - offset, y);
                unsigned int channel;
                for (channel = 0U; channel < image->channels; ++channel) {
                    uint8_t temporary = left[channel];
                    left[channel] = right[channel];
                    right[channel] = temporary;
                }
            }
        }
    } else {
        size_t offset;
        for (offset = 0U; offset < area_height / 2U; ++offset) {
            uint8_t *top = pit_image_pixel(image, area.left, area.top + offset);
            uint8_t *bottom = pit_image_pixel(image, area.left, area.bottom - offset);
            size_t row_bytes = area_width * image->channels;
            size_t index;
            for (index = 0U; index < row_bytes; ++index) {
                uint8_t temporary = top[index];
                top[index] = bottom[index];
                bottom[index] = temporary;
            }
        }
    }
    return PIT_OK;
}

PitStatus pit_copy_area(PitImage *image, PitRect source, PitPoint destination)
{
    size_t destination_x;
    size_t destination_y;
    size_t width;
    size_t height;
    size_t row_bytes;
    size_t byte_count;
    uint8_t *snapshot;
    size_t row;

    if (!pit_rect_is_valid(image, source)) {
        return PIT_OUT_OF_BOUNDS;
    }
    if (destination.x < 0 || destination.y < 0
        || (uint64_t)destination.x > SIZE_MAX || (uint64_t)destination.y > SIZE_MAX) {
        return PIT_OUT_OF_BOUNDS;
    }

    destination_x = (size_t)destination.x;
    destination_y = (size_t)destination.y;
    width = source.right - source.left + 1U;
    height = source.bottom - source.top + 1U;
    if (destination_x > image->width - width || destination_y > image->height - height) {
        return PIT_OUT_OF_BOUNDS;
    }

    row_bytes = width * image->channels;
    if (height > SIZE_MAX / row_bytes) {
        return PIT_ALLOCATION_FAILED;
    }
    byte_count = row_bytes * height;
    snapshot = malloc(byte_count);
    if (snapshot == NULL) {
        return PIT_ALLOCATION_FAILED;
    }

    /* Snapshot first so source and destination may overlap without corruption. */
    for (row = 0U; row < height; ++row) {
        const uint8_t *source_row = pit_image_const_pixel(image, source.left, source.top + row);
        memcpy(snapshot + row * row_bytes, source_row, row_bytes);
    }
    for (row = 0U; row < height; ++row) {
        uint8_t *destination_row = pit_image_pixel(image, destination_x, destination_y + row);
        memcpy(destination_row, snapshot + row * row_bytes, row_bytes);
    }

    free(snapshot);
    return PIT_OK;
}

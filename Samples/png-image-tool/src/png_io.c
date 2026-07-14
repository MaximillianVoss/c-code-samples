#include "png_image_tool/png_io.h"

#include <png.h>

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct PitPngErrorContext {
    char *buffer;
    size_t buffer_size;
} PitPngErrorContext;

static void pit_set_error(char *error, size_t error_size, const char *format, ...)
{
    va_list arguments;

    if (error == NULL || error_size == 0U) {
        return;
    }
    va_start(arguments, format);
    (void)vsnprintf(error, error_size, format, arguments);
    va_end(arguments);
}

static void pit_libpng_error(png_structp png, png_const_charp message)
{
    PitPngErrorContext *context = png_get_error_ptr(png);

    if (context != NULL) {
        pit_set_error(context->buffer, context->buffer_size, "libpng: %s", message);
    }
    png_longjmp(png, 1);
}

static void pit_libpng_warning(png_structp png, png_const_charp message)
{
    (void)png;
    (void)message;
}

static bool pit_png_metadata_is_supported(const PitPngImage *image)
{
    unsigned int expected_channels;

    if (image == NULL || image->image.pixels == NULL || image->bit_depth != 8
        || (image->color_type != PIT_PNG_COLOR_RGB && image->color_type != PIT_PNG_COLOR_RGBA)
        || (image->interlace_type != PIT_PNG_INTERLACE_NONE
            && image->interlace_type != PIT_PNG_INTERLACE_ADAM7)
        || image->compression_type != PNG_COMPRESSION_TYPE_BASE
        || image->filter_method != PNG_FILTER_TYPE_BASE) {
        return false;
    }
    expected_channels = image->color_type == PIT_PNG_COLOR_RGB ? 3U : 4U;
    return image->image.width <= SIZE_MAX / expected_channels
        && image->image.channels == expected_channels
        && image->image.stride == image->image.width * expected_channels;
}

PitStatus pit_png_read(
    const char *path,
    PitPngImage *image,
    char *error,
    size_t error_size)
{
    FILE *file;
    png_byte signature[8];
    png_structp png;
    png_infop info;
    PitPngErrorContext error_context = {error, error_size};
    volatile PitStatus status = PIT_PNG_ERROR;

    if (path == NULL || path[0] == '\0' || image == NULL) {
        return PIT_INVALID_ARGUMENT;
    }
    memset(image, 0, sizeof(*image));
    if (error != NULL && error_size > 0U) {
        error[0] = '\0';
    }

    file = fopen(path, "rb");
    if (file == NULL) {
        pit_set_error(error, error_size, "Cannot open input file: %s", path);
        return PIT_IO_ERROR;
    }
    if (fread(signature, 1U, sizeof(signature), file) != sizeof(signature)) {
        pit_set_error(error, error_size, "Input is too short to be a PNG file: %s", path);
        fclose(file);
        return PIT_UNSUPPORTED_FORMAT;
    }
    if (png_sig_cmp(signature, 0U, sizeof(signature)) != 0) {
        pit_set_error(error, error_size, "Input does not have a PNG signature: %s", path);
        fclose(file);
        return PIT_UNSUPPORTED_FORMAT;
    }

    png = png_create_read_struct(PNG_LIBPNG_VER_STRING, &error_context, pit_libpng_error, pit_libpng_warning);
    if (png == NULL) {
        fclose(file);
        return PIT_ALLOCATION_FAILED;
    }
    info = png_create_info_struct(png);
    if (info == NULL) {
        png_destroy_read_struct(&png, NULL, NULL);
        fclose(file);
        return PIT_ALLOCATION_FAILED;
    }

    /* libpng reports malformed streams through this recovery point. */
    if (setjmp(png_jmpbuf(png)) == 0) {
        png_uint_32 width;
        png_uint_32 height;
        int bit_depth;
        int color_type;
        int interlace_type;
        int compression_type;
        int filter_method;
        unsigned int channels;
        PitStatus create_status;
        int passes;
        int pass;

        png_init_io(png, file);
        png_set_sig_bytes(png, sizeof(signature));
        png_read_info(png, info);
        if (png_get_IHDR(
                png,
                info,
                &width,
                &height,
                &bit_depth,
                &color_type,
                &interlace_type,
                &compression_type,
                &filter_method)
            == 0U) {
            pit_set_error(error, error_size, "PNG file has no valid IHDR chunk");
            status = PIT_PNG_ERROR;
        } else if (width == 0U || height == 0U || bit_depth != 8
            || (color_type != PIT_PNG_COLOR_RGB && color_type != PIT_PNG_COLOR_RGBA)) {
            pit_set_error(error, error_size, "Only 8-bit RGB and RGBA PNG files are supported");
            status = PIT_UNSUPPORTED_FORMAT;
        } else {
            channels = color_type == PIT_PNG_COLOR_RGB ? 3U : 4U;
            create_status = pit_image_create(&image->image, (size_t)width, (size_t)height, channels);
            if (create_status == PIT_OK) {
                size_t y;

                image->bit_depth = bit_depth;
                image->color_type = color_type;
                image->interlace_type = interlace_type;
                image->compression_type = compression_type;
                image->filter_method = filter_method;

                passes = png_set_interlace_handling(png);
                if (passes < 1) {
                    passes = 1;
                }
                png_read_update_info(png, info);
                if ((size_t)png_get_rowbytes(png, info) != image->image.stride) {
                    pit_set_error(error, error_size, "Unexpected PNG row size");
                    status = PIT_UNSUPPORTED_FORMAT;
                } else {
                    for (pass = 0; pass < passes; ++pass) {
                        for (y = 0U; y < image->image.height; ++y) {
                            png_read_row(png, image->image.pixels + y * image->image.stride, NULL);
                        }
                    }
                    png_read_end(png, info);
                    status = PIT_OK;
                }
            } else {
                status = create_status;
            }
        }
    }

    png_destroy_read_struct(&png, &info, NULL);
    fclose(file);
    if (status != PIT_OK) {
        pit_png_image_destroy(image);
    }
    return (PitStatus)status;
}

PitStatus pit_png_write(
    const char *path,
    const PitPngImage *image,
    char *error,
    size_t error_size)
{
    FILE *file;
    png_structp png;
    png_infop info;
    png_bytepp rows;
    size_t y;
    PitPngErrorContext error_context = {error, error_size};
    volatile PitStatus status = PIT_PNG_ERROR;

    if (path == NULL || path[0] == '\0' || !pit_png_metadata_is_supported(image)) {
        return PIT_INVALID_ARGUMENT;
    }
    if (image->image.width > PNG_UINT_31_MAX || image->image.height > PNG_UINT_31_MAX) {
        pit_set_error(error, error_size, "Image dimensions exceed the PNG limit");
        return PIT_UNSUPPORTED_FORMAT;
    }
    if (error != NULL && error_size > 0U) {
        error[0] = '\0';
    }

    if (image->image.height > SIZE_MAX / sizeof(*rows)) {
        pit_set_error(error, error_size, "Image height is too large");
        return PIT_ALLOCATION_FAILED;
    }
    rows = malloc(image->image.height * sizeof(*rows));
    if (rows == NULL) {
        pit_set_error(error, error_size, "Cannot allocate PNG row table");
        return PIT_ALLOCATION_FAILED;
    }
    for (y = 0U; y < image->image.height; ++y) {
        rows[y] = (png_bytep)(image->image.pixels + y * image->image.stride);
    }

    file = fopen(path, "wb");
    if (file == NULL) {
        pit_set_error(error, error_size, "Cannot open output file: %s", path);
        free(rows);
        return PIT_IO_ERROR;
    }
    png = png_create_write_struct(PNG_LIBPNG_VER_STRING, &error_context, pit_libpng_error, pit_libpng_warning);
    if (png == NULL) {
        fclose(file);
        free(rows);
        return PIT_ALLOCATION_FAILED;
    }
    info = png_create_info_struct(png);
    if (info == NULL) {
        png_destroy_write_struct(&png, NULL);
        fclose(file);
        free(rows);
        return PIT_ALLOCATION_FAILED;
    }

    if (setjmp(png_jmpbuf(png)) == 0) {
        png_init_io(png, file);
        png_set_IHDR(
            png,
            info,
            (png_uint_32)image->image.width,
            (png_uint_32)image->image.height,
            image->bit_depth,
            image->color_type,
            image->interlace_type,
            image->compression_type,
            image->filter_method);
        png_write_info(png, info);
        png_write_image(png, rows);
        png_write_end(png, info);
        status = PIT_OK;
    }

    png_destroy_write_struct(&png, &info);
    free(rows);
    if (fclose(file) != 0 && status == PIT_OK) {
        pit_set_error(error, error_size, "Failed to close output file: %s", path);
        status = PIT_IO_ERROR;
    }
    return (PitStatus)status;
}

void pit_png_image_destroy(PitPngImage *image)
{
    if (image == NULL) {
        return;
    }
    pit_image_destroy(&image->image);
    image->bit_depth = 0;
    image->color_type = 0;
    image->interlace_type = 0;
    image->compression_type = 0;
    image->filter_method = 0;
}

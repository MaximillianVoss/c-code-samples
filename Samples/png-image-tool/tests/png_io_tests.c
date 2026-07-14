#include "png_image_tool/png_io.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int failures = 0;

#define CHECK(condition)                                                                            \
    do {                                                                                            \
        if (!(condition)) {                                                                         \
            fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #condition);                   \
            ++failures;                                                                             \
        }                                                                                           \
    } while (0)

static void fill_pattern(PitImage *image)
{
    size_t y;

    for (y = 0U; y < image->height; ++y) {
        size_t x;
        for (x = 0U; x < image->width; ++x) {
            uint8_t *pixel = pit_image_pixel(image, x, y);
            pixel[0] = (uint8_t)(x * 31U + y * 7U);
            pixel[1] = (uint8_t)(x * 11U + y * 29U);
            pixel[2] = (uint8_t)(x * 3U + y * 47U);
            if (image->channels == 4U) {
                pixel[3] = (uint8_t)(255U - x * 13U - y * 17U);
            }
        }
    }
}

static void test_round_trip(const char *path, int color_type, int interlace_type)
{
    PitPngImage original = {0};
    PitPngImage loaded = {0};
    char error[256];
    unsigned int channels = color_type == PIT_PNG_COLOR_RGB ? 3U : 4U;
    size_t byte_count;
    PitStatus status;

    (void)remove(path);
    CHECK(pit_image_create(&original.image, 5U, 4U, channels) == PIT_OK);
    original.bit_depth = 8;
    original.color_type = color_type;
    original.interlace_type = interlace_type;
    original.compression_type = 0;
    original.filter_method = 0;
    fill_pattern(&original.image);

    status = pit_png_write(path, &original, error, sizeof(error));
    CHECK(status == PIT_OK);
    if (status != PIT_OK) {
        fprintf(stderr, "pit_png_write: %s\n", error);
        pit_png_image_destroy(&original);
        (void)remove(path);
        return;
    }
    status = pit_png_read(path, &loaded, error, sizeof(error));
    CHECK(status == PIT_OK);
    if (status != PIT_OK) {
        fprintf(stderr, "pit_png_read: %s\n", error);
        pit_png_image_destroy(&original);
        (void)remove(path);
        return;
    }
    CHECK(loaded.image.width == original.image.width);
    CHECK(loaded.image.height == original.image.height);
    CHECK(loaded.image.channels == original.image.channels);
    CHECK(loaded.bit_depth == original.bit_depth);
    CHECK(loaded.color_type == original.color_type);
    CHECK(loaded.interlace_type == original.interlace_type);
    byte_count = original.image.stride * original.image.height;
    CHECK(memcmp(loaded.image.pixels, original.image.pixels, byte_count) == 0);

    pit_png_image_destroy(&loaded);
    pit_png_image_destroy(&original);
    CHECK(remove(path) == 0);
}

static void test_rejects_non_png(void)
{
    const char *path = "pit-invalid-signature.dat";
    PitPngImage image = {0};
    char error[256];
    FILE *file = fopen(path, "wb");

    CHECK(file != NULL);
    if (file != NULL) {
        CHECK(fwrite("not png", 1U, 7U, file) == 7U);
        CHECK(fclose(file) == 0);
        CHECK(pit_png_read(path, &image, error, sizeof(error)) == PIT_UNSUPPORTED_FORMAT);
        pit_png_image_destroy(&image);
        CHECK(remove(path) == 0);
    }
}

static int remove_iend_chunk(const char *path)
{
    FILE *file = fopen(path, "rb");
    long length;
    size_t truncated_size;
    unsigned char *bytes;
    int success = 0;

    if (file == NULL || fseek(file, 0L, SEEK_END) != 0) {
        if (file != NULL) {
            (void)fclose(file);
        }
        return 0;
    }
    length = ftell(file);
    if (length <= 12L || fseek(file, 0L, SEEK_SET) != 0) {
        (void)fclose(file);
        return 0;
    }
    truncated_size = (size_t)length - 12U;
    bytes = malloc(truncated_size);
    if (bytes == NULL) {
        (void)fclose(file);
        return 0;
    }
    if (fread(bytes, 1U, truncated_size, file) != truncated_size || fclose(file) != 0) {
        free(bytes);
        return 0;
    }

    file = fopen(path, "wb");
    if (file != NULL) {
        success = fwrite(bytes, 1U, truncated_size, file) == truncated_size;
        if (fclose(file) != 0) {
            success = 0;
        }
    }
    free(bytes);
    return success;
}

static void test_rejects_truncated_png_after_decoding(void)
{
    const char *path = "pit-truncated.png";
    PitPngImage original = {0};
    PitPngImage loaded = {0};
    char error[256];
    PitStatus status;

    (void)remove(path);
    CHECK(pit_image_create(&original.image, 3U, 2U, 3U) == PIT_OK);
    original.bit_depth = 8;
    original.color_type = PIT_PNG_COLOR_RGB;
    original.interlace_type = PIT_PNG_INTERLACE_NONE;
    original.compression_type = 0;
    original.filter_method = 0;
    fill_pattern(&original.image);

    status = pit_png_write(path, &original, error, sizeof(error));
    CHECK(status == PIT_OK);
    if (status == PIT_OK) {
        CHECK(remove_iend_chunk(path));
        status = pit_png_read(path, &loaded, error, sizeof(error));
        CHECK(status == PIT_PNG_ERROR);
        CHECK(loaded.image.pixels == NULL);
    }

    pit_png_image_destroy(&loaded);
    pit_png_image_destroy(&original);
    CHECK(remove(path) == 0);
}

int main(void)
{
    test_round_trip("pit-rgb-roundtrip.png", PIT_PNG_COLOR_RGB, PIT_PNG_INTERLACE_NONE);
    test_round_trip("pit-rgba-adam7-roundtrip.png", PIT_PNG_COLOR_RGBA, PIT_PNG_INTERLACE_ADAM7);
    test_rejects_non_png();
    test_rejects_truncated_png_after_decoding();

    if (failures != 0) {
        fprintf(stderr, "%d assertion(s) failed\n", failures);
        return 1;
    }
    puts("pit_png_io_tests: all checks passed");
    return 0;
}

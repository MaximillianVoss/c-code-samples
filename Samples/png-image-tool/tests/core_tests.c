#include "png_image_tool/cli.h"
#include "png_image_tool/image.h"
#include "png_image_tool/operations.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

static int failures = 0;

#define CHECK(condition)                                                                            \
    do {                                                                                            \
        if (!(condition)) {                                                                         \
            fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #condition);                   \
            ++failures;                                                                             \
        }                                                                                           \
    } while (0)

static void set_red(PitImage *image, size_t x, size_t y, uint8_t value)
{
    uint8_t *pixel = pit_image_pixel(image, x, y);
    pixel[0] = value;
}

static uint8_t get_red(const PitImage *image, size_t x, size_t y)
{
    return pit_image_const_pixel(image, x, y)[0];
}

static void test_image_allocation_guards(void)
{
    PitImage image = {0};
    uint8_t placeholder[4] = {0};
    PitImage malformed = {
        .width = SIZE_MAX,
        .height = 1U,
        .stride = SIZE_MAX - 2U,
        .channels = 3U,
        .pixels = placeholder};
    PitRect first_pixel = {0U, 0U, 0U, 0U};

    CHECK(pit_image_create(&image, 2U, 3U, 3U) == PIT_OK);
    CHECK(image.stride == 6U);
    CHECK(image.pixels != NULL);
    pit_image_destroy(&image);
    CHECK(image.pixels == NULL);
    CHECK(pit_image_create(&image, SIZE_MAX, 2U, 4U) == PIT_ALLOCATION_FAILED);
    CHECK(pit_image_create(&image, 1U, 1U, 2U) == PIT_INVALID_ARGUMENT);
    CHECK(pit_mirror_area(&malformed, first_pixel, PIT_MIRROR_LEFT_RIGHT) == PIT_OUT_OF_BOUNDS);
}

static void test_mirror_both_axes(void)
{
    PitImage image = {0};
    PitRect all = {0U, 0U, 2U, 1U};

    CHECK(pit_image_create(&image, 3U, 2U, 3U) == PIT_OK);
    set_red(&image, 0U, 0U, 1U);
    set_red(&image, 1U, 0U, 2U);
    set_red(&image, 2U, 0U, 3U);
    set_red(&image, 0U, 1U, 4U);
    set_red(&image, 1U, 1U, 5U);
    set_red(&image, 2U, 1U, 6U);

    CHECK(pit_mirror_area(&image, all, PIT_MIRROR_LEFT_RIGHT) == PIT_OK);
    CHECK(get_red(&image, 0U, 0U) == 3U);
    CHECK(get_red(&image, 1U, 0U) == 2U);
    CHECK(get_red(&image, 2U, 0U) == 1U);
    CHECK(get_red(&image, 0U, 1U) == 6U);

    CHECK(pit_mirror_area(&image, all, PIT_MIRROR_TOP_BOTTOM) == PIT_OK);
    CHECK(get_red(&image, 0U, 0U) == 6U);
    CHECK(get_red(&image, 2U, 1U) == 1U);
    pit_image_destroy(&image);
}

static void test_overlapping_copy_uses_snapshot(void)
{
    PitImage image = {0};
    PitRect source = {0U, 0U, 2U, 0U};
    PitPoint destination = {1, 0};

    CHECK(pit_image_create(&image, 4U, 1U, 3U) == PIT_OK);
    set_red(&image, 0U, 0U, 1U);
    set_red(&image, 1U, 0U, 2U);
    set_red(&image, 2U, 0U, 3U);
    set_red(&image, 3U, 0U, 4U);

    CHECK(pit_copy_area(&image, source, destination) == PIT_OK);
    CHECK(get_red(&image, 0U, 0U) == 1U);
    CHECK(get_red(&image, 1U, 0U) == 1U);
    CHECK(get_red(&image, 2U, 0U) == 2U);
    CHECK(get_red(&image, 3U, 0U) == 3U);
    pit_image_destroy(&image);
}

static void test_invalid_copy_does_not_modify_pixels(void)
{
    PitImage image = {0};
    PitRect source = {0U, 0U, 1U, 0U};
    PitPoint destination = {3, 0};
    uint8_t before[12];

    CHECK(pit_image_create(&image, 4U, 1U, 3U) == PIT_OK);
    memset(image.pixels, 42, image.stride * image.height);
    memcpy(before, image.pixels, sizeof(before));
    CHECK(pit_copy_area(&image, source, destination) == PIT_OUT_OF_BOUNDS);
    CHECK(memcmp(before, image.pixels, sizeof(before)) == 0);
    pit_image_destroy(&image);
}

static void test_circle_preserves_alpha(void)
{
    PitImage image = {0};
    PitPoint center = {2, 2};
    PitColor line = {0U, 255U, 0U};
    PitColor fill = {0U, 0U, 255U};
    size_t index;
    const uint8_t *pixel;

    CHECK(pit_image_create(&image, 5U, 5U, 4U) == PIT_OK);
    for (index = 0U; index < image.width * image.height; ++index) {
        image.pixels[index * 4U + 3U] = 77U;
    }
    CHECK(pit_draw_circle(&image, center, 1U, 1U, line, true, fill) == PIT_OK);

    pixel = pit_image_const_pixel(&image, 2U, 2U);
    CHECK(pixel[0] == 0U && pixel[1] == 0U && pixel[2] == 255U && pixel[3] == 77U);
    pixel = pit_image_const_pixel(&image, 3U, 2U);
    CHECK(pixel[0] == 0U && pixel[1] == 255U && pixel[2] == 0U && pixel[3] == 77U);
    pixel = pit_image_const_pixel(&image, 0U, 0U);
    CHECK(pixel[0] == 0U && pixel[1] == 0U && pixel[2] == 0U && pixel[3] == 77U);
    pit_image_destroy(&image);
}

static void test_cli_validation(void)
{
    PitCliOptions options;
    char error[160];
    char *valid[] = {
        "tool", "--input", "in.png", "--output", "out.png", "--circle", "--center",
        "2.3", "--radius", "4", "--color", "1.2.3"};
    char *bad_color[] = {"tool", "--input", "in.png", "--circle", "--center", "2.3",
        "--radius", "4", "--color", "1.2.256"};
    char *missing_axis[] = {"tool", "--input", "in.png", "--mirror", "--left_up", "0.0",
        "--right_down", "2.2"};
    char *reversed[] = {"tool", "--input", "in.png", "--copy", "--left_up", "3.3",
        "--right_down", "2.2", "--dest_left_up", "0.0"};

    CHECK(pit_cli_parse((int)(sizeof(valid) / sizeof(valid[0])), valid, &options, error, sizeof(error))
        == PIT_OK);
    CHECK(options.draw_circle);
    CHECK(options.center.x == 2 && options.center.y == 3);
    CHECK(options.radius == 4U);
    CHECK(options.line_color.red == 1U && options.line_color.green == 2U
        && options.line_color.blue == 3U);

    CHECK(pit_cli_parse(
              (int)(sizeof(bad_color) / sizeof(bad_color[0])),
              bad_color,
              &options,
              error,
              sizeof(error))
        == PIT_INVALID_ARGUMENT);
    CHECK(pit_cli_parse(
              (int)(sizeof(missing_axis) / sizeof(missing_axis[0])),
              missing_axis,
              &options,
              error,
              sizeof(error))
        == PIT_INVALID_ARGUMENT);
    CHECK(pit_cli_parse(
              (int)(sizeof(reversed) / sizeof(reversed[0])),
              reversed,
              &options,
              error,
              sizeof(error))
        == PIT_INVALID_ARGUMENT);
}

int main(void)
{
    test_image_allocation_guards();
    test_mirror_both_axes();
    test_overlapping_copy_uses_snapshot();
    test_invalid_copy_does_not_modify_pixels();
    test_circle_preserves_alpha();
    test_cli_validation();

    if (failures != 0) {
        fprintf(stderr, "%d assertion(s) failed\n", failures);
        return 1;
    }
    puts("pit_core_tests: all checks passed");
    return 0;
}

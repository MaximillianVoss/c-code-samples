#include "png_image_tool/cli.h"
#include "png_image_tool/png_io.h"

#include <stdio.h>

static int pit_report_operation_error(const char *operation, PitStatus status)
{
    fprintf(stderr, "Error: %s failed: %s\n", operation, pit_status_message(status));
    return 1;
}

int main(int argc, char *argv[])
{
    PitCliOptions options;
    PitPngImage png_image = {0};
    char error[256];
    PitStatus status;
    int result = 0;

    status = pit_cli_parse(argc, argv, &options, error, sizeof(error));
    if (status != PIT_OK) {
        fprintf(stderr, "Error: %s\n\n", error[0] != '\0' ? error : pit_status_message(status));
        pit_cli_print_usage(stderr, argv[0]);
        return 2;
    }
    if (options.show_help) {
        pit_cli_print_usage(stdout, argv[0]);
        return 0;
    }

    status = pit_png_read(options.input_path, &png_image, error, sizeof(error));
    if (status != PIT_OK) {
        fprintf(stderr, "Error: %s\n", error[0] != '\0' ? error : pit_status_message(status));
        return 1;
    }

    if (options.show_info) {
        printf("PNG Info:\n");
        printf("  Width: %zu\n", png_image.image.width);
        printf("  Height: %zu\n", png_image.image.height);
        printf("  Bit Depth: %d\n", png_image.bit_depth);
        printf("  Color Type: %d\n", png_image.color_type);
        printf("  Channels: %u\n", png_image.image.channels);
        pit_png_image_destroy(&png_image);
        return 0;
    }

    if (options.draw_circle) {
        status = pit_draw_circle(
            &png_image.image,
            options.center,
            options.radius,
            options.thickness,
            options.line_color,
            options.fill_circle,
            options.fill_color);
        if (status != PIT_OK) {
            result = pit_report_operation_error("circle", status);
            goto cleanup;
        }
    }
    if (options.mirror) {
        status = pit_mirror_area(&png_image.image, options.area, options.mirror_axis);
        if (status != PIT_OK) {
            result = pit_report_operation_error("mirror", status);
            goto cleanup;
        }
    }
    if (options.copy) {
        status = pit_copy_area(&png_image.image, options.area, options.destination);
        if (status != PIT_OK) {
            result = pit_report_operation_error("copy", status);
            goto cleanup;
        }
    }

    status = pit_png_write(options.output_path, &png_image, error, sizeof(error));
    if (status != PIT_OK) {
        fprintf(stderr, "Error: %s\n", error[0] != '\0' ? error : pit_status_message(status));
        result = 1;
    }

cleanup:
    pit_png_image_destroy(&png_image);
    return result;
}

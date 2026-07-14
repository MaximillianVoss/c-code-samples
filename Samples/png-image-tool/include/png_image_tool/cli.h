#ifndef PNG_IMAGE_TOOL_CLI_H
#define PNG_IMAGE_TOOL_CLI_H

#include "png_image_tool/operations.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

typedef struct PitCliOptions {
    const char *input_path;
    const char *output_path;
    bool show_help;
    bool show_info;

    bool draw_circle;
    PitPoint center;
    unsigned int radius;
    unsigned int thickness;
    PitColor line_color;
    bool fill_circle;
    PitColor fill_color;

    bool mirror;
    PitMirrorAxis mirror_axis;
    bool copy;
    PitRect area;
    PitPoint destination;
} PitCliOptions;

PitStatus pit_cli_parse(
    int argc,
    char *const argv[],
    PitCliOptions *options,
    char *error,
    size_t error_size);

void pit_cli_print_usage(FILE *stream, const char *program_name);

#endif

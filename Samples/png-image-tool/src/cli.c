#include "png_image_tool/cli.h"

#include <errno.h>
#include <limits.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

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

static bool pit_parse_point(const char *text, PitPoint *point)
{
    char *separator;
    char *end;
    long long x;
    long long y;

    if (text == NULL || point == NULL) {
        return false;
    }
    errno = 0;
    x = strtoll(text, &separator, 10);
    if (errno == ERANGE || separator == text || *separator != '.') {
        return false;
    }
    errno = 0;
    y = strtoll(separator + 1, &end, 10);
    if (errno == ERANGE || end == separator + 1 || *end != '\0' || x < 0 || y < 0) {
        return false;
    }
    point->x = (int64_t)x;
    point->y = (int64_t)y;
    return true;
}

static bool pit_parse_color(const char *text, PitColor *color)
{
    char *first_separator;
    char *second_separator;
    char *end;
    long red;
    long green;
    long blue;

    if (text == NULL || color == NULL) {
        return false;
    }
    errno = 0;
    red = strtol(text, &first_separator, 10);
    if (errno == ERANGE || first_separator == text || *first_separator != '.') {
        return false;
    }
    errno = 0;
    green = strtol(first_separator + 1, &second_separator, 10);
    if (errno == ERANGE || second_separator == first_separator + 1 || *second_separator != '.') {
        return false;
    }
    errno = 0;
    blue = strtol(second_separator + 1, &end, 10);
    if (errno == ERANGE || end == second_separator + 1 || *end != '\0'
        || red < 0 || red > 255 || green < 0 || green > 255 || blue < 0 || blue > 255) {
        return false;
    }
    color->red = (uint8_t)red;
    color->green = (uint8_t)green;
    color->blue = (uint8_t)blue;
    return true;
}

static bool pit_parse_positive_unsigned(const char *text, unsigned int *value)
{
    char *end;
    unsigned long long parsed;

    if (text == NULL || value == NULL || text[0] == '-') {
        return false;
    }
    errno = 0;
    parsed = strtoull(text, &end, 10);
    if (errno == ERANGE || end == text || *end != '\0' || parsed == 0U || parsed > UINT_MAX) {
        return false;
    }
    *value = (unsigned int)parsed;
    return true;
}

static const char *pit_next_value(
    int argc,
    char *const argv[],
    int *index,
    char *error,
    size_t error_size)
{
    if (*index + 1 >= argc) {
        pit_set_error(error, error_size, "Option %s requires a value", argv[*index]);
        return NULL;
    }
    ++(*index);
    return argv[*index];
}

PitStatus pit_cli_parse(
    int argc,
    char *const argv[],
    PitCliOptions *options,
    char *error,
    size_t error_size)
{
    bool center_set = false;
    bool radius_set = false;
    bool axis_set = false;
    bool left_up_set = false;
    bool right_down_set = false;
    bool destination_set = false;
    PitPoint left_up = {0, 0};
    PitPoint right_down = {0, 0};
    int index;

    if (options == NULL || argv == NULL || argc < 1) {
        return PIT_INVALID_ARGUMENT;
    }
    memset(options, 0, sizeof(*options));
    options->output_path = "out.png";
    options->thickness = 1U;
    options->line_color = (PitColor){255U, 0U, 0U};
    options->fill_color = (PitColor){255U, 255U, 255U};
    if (error != NULL && error_size > 0U) {
        error[0] = '\0';
    }

    for (index = 1; index < argc; ++index) {
        const char *argument = argv[index];
        const char *value;

        if (strcmp(argument, "--help") == 0 || strcmp(argument, "-h") == 0) {
            options->show_help = true;
        } else if (strcmp(argument, "--input") == 0 || strcmp(argument, "-i") == 0) {
            value = pit_next_value(argc, argv, &index, error, error_size);
            if (value == NULL) {
                return PIT_INVALID_ARGUMENT;
            }
            options->input_path = value;
        } else if (strcmp(argument, "--output") == 0 || strcmp(argument, "-o") == 0) {
            value = pit_next_value(argc, argv, &index, error, error_size);
            if (value == NULL) {
                return PIT_INVALID_ARGUMENT;
            }
            options->output_path = value;
        } else if (strcmp(argument, "--info") == 0 || strcmp(argument, "-p") == 0) {
            options->show_info = true;
        } else if (strcmp(argument, "--circle") == 0) {
            options->draw_circle = true;
        } else if (strcmp(argument, "--center") == 0) {
            value = pit_next_value(argc, argv, &index, error, error_size);
            if (value == NULL || !pit_parse_point(value, &options->center)) {
                pit_set_error(error, error_size, "Invalid --center value; expected x.y");
                return PIT_INVALID_ARGUMENT;
            }
            center_set = true;
        } else if (strcmp(argument, "--radius") == 0) {
            value = pit_next_value(argc, argv, &index, error, error_size);
            if (value == NULL || !pit_parse_positive_unsigned(value, &options->radius)) {
                pit_set_error(error, error_size, "Invalid --radius value; expected a positive integer");
                return PIT_INVALID_ARGUMENT;
            }
            radius_set = true;
        } else if (strcmp(argument, "--thickness") == 0) {
            value = pit_next_value(argc, argv, &index, error, error_size);
            if (value == NULL || !pit_parse_positive_unsigned(value, &options->thickness)) {
                pit_set_error(error, error_size, "Invalid --thickness value; expected a positive integer");
                return PIT_INVALID_ARGUMENT;
            }
        } else if (strcmp(argument, "--color") == 0) {
            value = pit_next_value(argc, argv, &index, error, error_size);
            if (value == NULL || !pit_parse_color(value, &options->line_color)) {
                pit_set_error(error, error_size, "Invalid --color value; expected r.g.b in range 0..255");
                return PIT_INVALID_ARGUMENT;
            }
        } else if (strcmp(argument, "--fill") == 0) {
            options->fill_circle = true;
        } else if (strcmp(argument, "--fill_color") == 0) {
            value = pit_next_value(argc, argv, &index, error, error_size);
            if (value == NULL || !pit_parse_color(value, &options->fill_color)) {
                pit_set_error(error, error_size, "Invalid --fill_color value; expected r.g.b in range 0..255");
                return PIT_INVALID_ARGUMENT;
            }
        } else if (strcmp(argument, "--mirror") == 0) {
            options->mirror = true;
        } else if (strcmp(argument, "--axis") == 0) {
            value = pit_next_value(argc, argv, &index, error, error_size);
            if (value == NULL || value[1] != '\0' || (value[0] != 'x' && value[0] != 'y')) {
                pit_set_error(error, error_size, "Invalid --axis value; expected x or y");
                return PIT_INVALID_ARGUMENT;
            }
            options->mirror_axis = value[0] == 'x' ? PIT_MIRROR_LEFT_RIGHT : PIT_MIRROR_TOP_BOTTOM;
            axis_set = true;
        } else if (strcmp(argument, "--left_up") == 0) {
            value = pit_next_value(argc, argv, &index, error, error_size);
            if (value == NULL || !pit_parse_point(value, &left_up)) {
                pit_set_error(error, error_size, "Invalid --left_up value; expected left.top");
                return PIT_INVALID_ARGUMENT;
            }
            left_up_set = true;
        } else if (strcmp(argument, "--right_down") == 0) {
            value = pit_next_value(argc, argv, &index, error, error_size);
            if (value == NULL || !pit_parse_point(value, &right_down)) {
                pit_set_error(error, error_size, "Invalid --right_down value; expected right.bottom");
                return PIT_INVALID_ARGUMENT;
            }
            right_down_set = true;
        } else if (strcmp(argument, "--copy") == 0) {
            options->copy = true;
        } else if (strcmp(argument, "--dest_left_up") == 0) {
            value = pit_next_value(argc, argv, &index, error, error_size);
            if (value == NULL || !pit_parse_point(value, &options->destination)) {
                pit_set_error(error, error_size, "Invalid --dest_left_up value; expected left.top");
                return PIT_INVALID_ARGUMENT;
            }
            destination_set = true;
        } else {
            pit_set_error(error, error_size, "Unknown option: %s", argument);
            return PIT_INVALID_ARGUMENT;
        }
    }

    if (options->show_help) {
        return PIT_OK;
    }
    if (options->input_path == NULL || options->input_path[0] == '\0') {
        pit_set_error(error, error_size, "--input is required");
        return PIT_INVALID_ARGUMENT;
    }
    if (options->output_path == NULL || options->output_path[0] == '\0') {
        pit_set_error(error, error_size, "--output cannot be empty");
        return PIT_INVALID_ARGUMENT;
    }
    if (options->show_info && (options->draw_circle || options->mirror || options->copy)) {
        pit_set_error(error, error_size, "--info cannot be combined with image modifications");
        return PIT_INVALID_ARGUMENT;
    }
    if (options->draw_circle && (!center_set || !radius_set)) {
        pit_set_error(error, error_size, "--circle requires --center and --radius");
        return PIT_INVALID_ARGUMENT;
    }
    if (options->mirror && !axis_set) {
        pit_set_error(error, error_size, "--mirror requires --axis");
        return PIT_INVALID_ARGUMENT;
    }
    if ((options->mirror || options->copy) && (!left_up_set || !right_down_set)) {
        pit_set_error(error, error_size, "--mirror and --copy require --left_up and --right_down");
        return PIT_INVALID_ARGUMENT;
    }
    if (options->copy && !destination_set) {
        pit_set_error(error, error_size, "--copy requires --dest_left_up");
        return PIT_INVALID_ARGUMENT;
    }
    if (left_up_set && right_down_set) {
        if (left_up.x > right_down.x || left_up.y > right_down.y) {
            pit_set_error(error, error_size, "The rectangle corners are reversed");
            return PIT_INVALID_ARGUMENT;
        }
        if ((uint64_t)left_up.x > SIZE_MAX || (uint64_t)left_up.y > SIZE_MAX
            || (uint64_t)right_down.x > SIZE_MAX || (uint64_t)right_down.y > SIZE_MAX) {
            pit_set_error(error, error_size, "The rectangle is too large for this platform");
            return PIT_INVALID_ARGUMENT;
        }
        options->area = (PitRect){
            (size_t)left_up.x,
            (size_t)left_up.y,
            (size_t)right_down.x,
            (size_t)right_down.y};
    }
    return PIT_OK;
}

void pit_cli_print_usage(FILE *stream, const char *program_name)
{
    const char *name = program_name != NULL ? program_name : "png-image-tool";

    fprintf(stream, "Usage: %s --input FILE [--output FILE] [operations]\n", name);
    fprintf(stream, "       %s --input FILE --info\n", name);
    fprintf(stream, "\nOperations:\n");
    fprintf(stream, "  --circle --center X.Y --radius N [--thickness N]\n");
    fprintf(stream, "           [--color R.G.B] [--fill --fill_color R.G.B]\n");
    fprintf(stream, "  --mirror --axis x|y --left_up X.Y --right_down X.Y\n");
    fprintf(stream, "  --copy --left_up X.Y --right_down X.Y --dest_left_up X.Y\n");
}

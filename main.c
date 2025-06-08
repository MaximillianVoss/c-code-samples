#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <getopt.h>

#include "types.h"
#include "utils.h"
#include "circle.h"
#include "mirror.h"
#include "copy.h"

int main(int argc, char *argv[])
{
    const char *input = NULL;
    const char *output = "out.png";
    bool info_flag = false;

    // параметры окружности
    bool draw_circle_flag = false;
    Point center = {0, 0};
    int radius = 0, thickness = 1;
    Color color = {255, 0, 0}, fill_color = {255, 255, 255};
    bool fill = false;

    // зеркалирование
    bool do_mirror = false;
    char mirror_axis = ' ';
    Point mirror_left_up = {0, 0}, mirror_right_down = {0, 0};

    // копирование
    bool do_copy = false;
    Point copy_left_up = {0, 0}, copy_right_down = {0, 0}, copy_dest_left_up = {0, 0};

    const struct option options[] = {
        {"input", required_argument, NULL, 'i'},
        {"output", required_argument, NULL, 'o'},
        {"info", no_argument, NULL, 'p'},
        {"circle", no_argument, NULL, 1},
        {"center", required_argument, NULL, 2},
        {"radius", required_argument, NULL, 3},
        {"thickness", required_argument, NULL, 4},
        {"color", required_argument, NULL, 5},
        {"fill", no_argument, NULL, 6},
        {"fill_color", required_argument, NULL, 7},
        {"mirror", no_argument, NULL, 8},
        {"axis", required_argument, NULL, 9},
        {"left_up", required_argument, NULL, 10},
        {"right_down", required_argument, NULL, 11},
        {"copy", no_argument, NULL, 12},
        {"dest_left_up", required_argument, NULL, 13},
        {NULL, 0, NULL, 0}};

    int opt;
    while ((opt = getopt_long(argc, argv, "i:o:p", options, NULL)) != -1)
    {
        switch (opt)
        {
        case 'i':
            input = optarg;
            break;
        case 'o':
            output = optarg;
            break;
        case 'p':
            info_flag = true;
            break;

        case 1:
            draw_circle_flag = true;
            break;
        case 2:
            sscanf(optarg, "%d.%d", &center.x, &center.y);
            break;
        case 3:
            radius = atoi(optarg);
            break;
        case 4:
            thickness = atoi(optarg);
            break;
        case 5:
            sscanf(optarg, "%d.%d.%d", &color.r, &color.g, &color.b);
            break;
        case 6:
            fill = true;
            break;
        case 7:
            sscanf(optarg, "%d.%d.%d", &fill_color.r, &fill_color.g, &fill_color.b);
            break;

        case 8:
            do_mirror = true;
            break;
        case 9:
            if (strlen(optarg) == 1 && (optarg[0] == 'x' || optarg[0] == 'y'))
            {
                mirror_axis = optarg[0];
            }
            else
            {
                handle_error("Axis must be 'x' or 'y'");
                return 1;
            }
            break;
        case 10:
            sscanf(optarg, "%d.%d", &mirror_left_up.x, &mirror_left_up.y);
            copy_left_up = mirror_left_up;
            break;
        case 11:
            sscanf(optarg, "%d.%d", &mirror_right_down.x, &mirror_right_down.y);
            copy_right_down = mirror_right_down;
            break;
        case 12:
            do_copy = true;
            break;
        case 13:
            sscanf(optarg, "%d.%d", &copy_dest_left_up.x, &copy_dest_left_up.y);
            break;

        default:
            fprintf(stderr, "Usage: %s --input file.png [...options...]\n", argv[0]);
            return 1;
        }
    }

    if (!input)
    {
        handle_error("No input file specified");
        return 1;
    }

    if (!is_png_file(input))
    {
        handle_error("Input file is not a valid PNG");
        return 1;
    }

    PngImage img;
    if (!read_png_file(input, &img))
        return 1;

    if (info_flag)
    {
        print_png_info(&img);
    }
    else
    {
        if (draw_circle_flag)
            draw_circle(&img, center, radius, thickness, color, fill, fill_color);

        if (do_mirror)
            mirror_area(&img, mirror_left_up, mirror_right_down, mirror_axis);

        if (do_copy)
            copy_area(&img, copy_left_up, copy_right_down, copy_dest_left_up);

        if (!write_png_file(output, &img))
        {
            free_png_image(&img);
            return 1;
        }
    }

    free_png_image(&img);
    return 0;
}

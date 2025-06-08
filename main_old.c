#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <getopt.h>
#include <png.h>

#pragma region Структуры
typedef struct
{
    int x;
    int y;
} Point;

typedef struct
{
    int r;
    int g;
    int b;
} Color;
#pragma endregion

void print_help(const char *program_name);
void print_png_info(png_structp png_ptr, png_infop info_ptr);
bool is_png_file(const char *filename);
void handle_error(const char *message);
png_bytep *read_png_file(const char *filename, int *width, int *height, png_byte *color_type, png_byte *bit_depth);
void write_png_file(const char *filename, int width, int height, png_byte color_type, png_byte bit_depth, png_bytep *row_pointers);
void draw_circle(png_bytep *row_pointers, int width, int height, Point center, int radius, int thickness, Color color, bool fill, Color fill_color);
void mirror_area(png_bytep *row_pointers, int width, int height, Point left_up, Point right_down, char axis);
void copy_area(png_bytep *row_pointers, int width, int height, Point left_up, Point right_down, Point dest_left_up);

bool rects_intersect(Point a_top_left, Point a_bottom_right, Point b_top_left, Point b_bottom_right)
{
    return !(a_top_left.x > b_bottom_right.x ||
             a_bottom_right.x < b_top_left.x ||
             a_top_left.y > b_bottom_right.y ||
             a_bottom_right.y < b_top_left.y);
}

int main(int argc, char **argv)
{
    printf("Course work for option 4.13, created by Poleatev Mikhail.\n");
    const char *input_file = NULL;
    const char *output_file = "out.png";

    bool draw_circle_flag = false;
    Point circle_center = {0, 0};
    int circle_radius = 0;
    int circle_thickness = 1;
    Color circle_color = {0, 0, 0};
    bool circle_fill = false;
    Color circle_fill_color = {255, 255, 255};

    bool mirror_flag = false;
    char mirror_axis = ' ';
    Point mirror_left_up = {0, 0};
    Point mirror_right_down = {0, 0};

    bool copy_flag = false;
    Point copy_left_up = {0, 0};
    Point copy_right_down = {0, 0};
    Point copy_dest_left_up = {0, 0};

    bool info_flag = false;

    int opt;

    const char *short_options = "ho:i";
    const struct option long_options[] = {
        {"help", no_argument, NULL, 'h'},
        {"output", required_argument, NULL, 'o'},
        {"info", no_argument, NULL, 'i'},
        {"circle", no_argument, NULL, 0},
        {"center", required_argument, NULL, 1},
        {"radius", required_argument, NULL, 2},
        {"thickness", required_argument, NULL, 3},
        {"color", required_argument, NULL, 4},
        {"fill", no_argument, NULL, 5},
        {"fill_color", required_argument, NULL, 6},
        {"mirror", no_argument, NULL, 7},
        {"axis", required_argument, NULL, 8},
        {"left_up", required_argument, NULL, 9},
        {"right_down", required_argument, NULL, 10},
        {"copy", no_argument, NULL, 11},
        {"dest_left_up", required_argument, NULL, 12},
        {"input", required_argument, NULL, 13},
        {NULL, 0, NULL, 0}};
    int option_index = 0;
    while ((opt = getopt_long(argc, argv, short_options, long_options, &option_index)) != -1)
    {
        switch (opt)
        {
        case 'h':
            print_help(argv[0]);
            return 0;
        case 'o':
            output_file = optarg;
            break;
        case 'i':
            info_flag = true;
            break;
        case 0:
            draw_circle_flag = true;
            break;
        case 1:
            if (sscanf(optarg, "%d.%d", &circle_center.x, &circle_center.y) != 2)
            {
                handle_error("Invalid center format. Use x.y");
                return 1;
            }
            break;
        case 2:
            circle_radius = atoi(optarg);
            break;
        case 3:
            circle_thickness = atoi(optarg);
            break;
        case 4:
            if (sscanf(optarg, "%d.%d.%d", &circle_color.r, &circle_color.g, &circle_color.b) != 3)
            {
                handle_error("Invalid color format. Use rrr.ggg.bbb");
                return 1;
            }
            break;
        case 5:
            circle_fill = true;
            break;
        case 6:
            if (sscanf(optarg, "%d.%d.%d", &circle_fill_color.r, &circle_fill_color.g, &circle_fill_color.b) != 3)
            {
                handle_error("Invalid fill_color format. Use rrr.ggg.bbb");
                return 1;
            }
            break;
        case 7:
            mirror_flag = true;
            break;
        case 8:
            if (strlen(optarg) != 1 || (optarg[0] != 'x' && optarg[0] != 'y'))
            {
                handle_error("Invalid axis value. Use 'x' or 'y'");
                return 1;
            }
            mirror_axis = optarg[0];
            break;
        case 9:
            if (sscanf(optarg, "%d.%d", &mirror_left_up.x, &mirror_left_up.y) != 2)
            {
                handle_error("Invalid left_up format. Use left.up");
                return 1;
            }
            break;
        case 10:
            if (sscanf(optarg, "%d.%d", &mirror_right_down.x, &mirror_right_down.y) != 2)
            {
                handle_error("Invalid right_down format. Use right.down");
                return 1;
            }
            break;
        case 11:
            copy_flag = true;
            break;
        case 12:
            if (sscanf(optarg, "%d.%d", &copy_dest_left_up.x, &copy_dest_left_up.y) != 2)
            {
                handle_error("Invalid dest_left_up format. Use left.up");
                return 1;
            }
            break;
        case 13:
            input_file = optarg;
            break;
        case '?':
            handle_error("Invalid option");
            return 1;
        default:
            return 1;
        }
    }

    if (input_file == NULL)
    {
        print_help(argv[0]);
        handle_error("No input file specified");
        return 1;
    }

    if (!is_png_file(input_file))
    {
        handle_error("Input file is not a PNG file");
        return 1;
    }

    int width, height;
    png_byte color_type;
    png_byte bit_depth;
    png_bytep *row_pointers = read_png_file(input_file, &width, &height, &color_type, &bit_depth);
    if (row_pointers == NULL)
    {
        return 1;
    }
    if (info_flag)
    {
        png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (!png_ptr)
        {
            handle_error("png_create_read_struct failed");
            return 1;
        }
        png_infop info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr)
        {
            handle_error("png_create_info_struct failed");
            png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
            return 1;
        }

        FILE *fp = fopen(input_file, "rb");
        if (!fp)
        {
            handle_error("File could not be opened for reading");
            png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
            return 1;
        }

        png_init_io(png_ptr, fp);
        png_read_info(png_ptr, info_ptr);
        print_png_info(png_ptr, info_ptr);

        fclose(fp);
        png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);

        return 0;
    }

    if (draw_circle_flag && (circle_radius > 0 && circle_thickness > 0))
    {
        draw_circle(row_pointers, width, height, circle_center, circle_radius, circle_thickness, circle_color, circle_fill, circle_fill_color);
    }
    else if (mirror_flag)
    {
        mirror_area(row_pointers, width, height, mirror_left_up, mirror_right_down, mirror_axis);
    }
    else if (copy_flag)
    {
        copy_area(row_pointers, width, height, copy_left_up, copy_right_down, copy_dest_left_up);
    }

    write_png_file(output_file, width, height, color_type, bit_depth, row_pointers);

    for (int y = 0; y < height; y++)
    {
        free(row_pointers[y]);
    }
    free(row_pointers);

    return 0;
}

void draw_circle(png_bytep *row_pointers, int width, int height, Point center, int radius, int thickness, Color color, bool fill, Color fill_color)
{
    if (radius <= 0 || thickness <= 0)
    {
        return;
    }

    int x, y;
    int start_x = fmax(center.x - (radius + thickness), 0);
    int end_x = fmin(center.x + (radius + thickness), width - 1);
    int start_y = fmax(center.y - (radius + thickness), 0);
    int end_y = fmin(center.y + (radius + thickness), height - 1);

    for (y = start_y; y <= end_y; y++)
    {
        for (x = start_x; x <= end_x; x++)
        {
            double distance = sqrt(pow(x - center.x, 2) + pow(y - center.y, 2));
            if (fill)
            {
                if (distance <= radius)
                {
                    png_bytep pixel = &(row_pointers[y][x * 3]);
                    pixel[0] = fill_color.r;
                    pixel[1] = fill_color.g;
                    pixel[2] = fill_color.b;
                }
            }
            if (distance >= radius - thickness / 2.0 && distance <= radius + thickness / 2.0)
            {
                png_bytep pixel = &(row_pointers[y][x * 3]);
                pixel[0] = color.r;
                pixel[1] = color.g;
                pixel[2] = color.b;
            }
        }
    }
}

void mirror_area(png_bytep *row_pointers, int width, int height, Point left_up, Point right_down, char axis)
{
    int x, y;

    int left_x = (int)left_up.x;
    int up_y = (int)left_up.y;
    int right_x = (int)right_down.x;
    int down_y = (int)right_down.y;

    if (left_x < 0 || left_x >= width || up_y < 0 || up_y >= height ||
        right_x < 0 || right_x >= width || down_y < 0 || down_y >= height ||
        left_x > right_x || up_y > down_y)
    {
        handle_error("Invalid mirror area coordinates");
        return;
    }

    if (axis == 'x')
    {
        for (y = up_y; y <= down_y; y++)
        {
            for (x = left_x; x <= (left_x + right_x) / 2; x++)
            {
                png_bytep pixel1 = &(row_pointers[y][x * 3]);
                png_bytep pixel2 = &(row_pointers[y][(right_x - (x - left_x)) * 3]);

                for (int c = 0; c < 3; c++)
                {
                    png_byte temp = pixel1[c];
                    pixel1[c] = pixel2[c];
                    pixel2[c] = temp;
                }
            }
        }
    }
    else if (axis == 'y')
    {
        for (x = left_x; x <= right_x; x++)
        {
            for (y = up_y; y <= (up_y + down_y) / 2; y++)
            {
                png_bytep pixel1 = &(row_pointers[y][x * 3]);
                png_bytep pixel2 = &(row_pointers[down_y - (y - up_y)][x * 3]);

                for (int c = 0; c < 3; c++)
                {
                    png_byte temp = pixel1[c];
                    pixel1[c] = pixel2[c];
                    pixel2[c] = temp;
                }
            }
        }
    }
    else
    {
        handle_error("Invalid axis for mirroring");
    }
}

void copy_area(png_bytep *row_pointers, int width, int height, Point left_up, Point right_down, Point dest_left_up)
{
    if (left_up.x < 0 || left_up.y < 0 || right_down.x >= width || right_down.y >= height)
    {
        handle_error("Source coordinates are out of bounds");
        return;
    }

    int src_width = right_down.x - left_up.x + 1;
    int src_height = right_down.y - left_up.y + 1;

    int dest_x_end = dest_left_up.x + src_width;
    int dest_y_end = dest_left_up.y + src_height;

    if (dest_left_up.x < 0)
    {
        src_width += dest_left_up.x;
        dest_left_up.x = 0;
    }
    if (dest_left_up.y < 0)
    {
        src_height += dest_left_up.y;
        dest_left_up.y = 0;
    }
    if (dest_x_end > width)
    {
        src_width -= (dest_x_end - width);
    }
    if (dest_y_end > height)
    {
        src_height -= (dest_y_end - height);
    }

    if (src_width <= 0 || src_height <= 0)
    {
        handle_error("No area to copy");
        return;
    }

    png_bytep *temp_buffer = malloc(src_height * sizeof(png_bytep));
    for (int i = 0; i < src_height; i++)
    {
        temp_buffer[i] = malloc(src_width * 3);
    }

    for (int y = 0; y < src_height; y++)
    {
        for (int x = 0; x < src_width; x++)
        {
            png_bytep src_pixel = &(row_pointers[left_up.y + y][(left_up.x + x) * 3]);
            temp_buffer[y][x * 3] = src_pixel[0];
            temp_buffer[y][x * 3 + 1] = src_pixel[1];
            temp_buffer[y][x * 3 + 2] = src_pixel[2];
        }
    }

    for (int y = 0; y < src_height; y++)
    {
        for (int x = 0; x < src_width; x++)
        {
            png_bytep dest_pixel = &(row_pointers[dest_left_up.y + y][(dest_left_up.x + x) * 3]);
            dest_pixel[0] = temp_buffer[y][x * 3];
            dest_pixel[1] = temp_buffer[y][x * 3 + 1];
            dest_pixel[2] = temp_buffer[y][x * 3 + 2];
        }
    }

    for (int i = 0; i < src_height; i++)
    {
        free(temp_buffer[i]);
    }
    free(temp_buffer);
}

void print_help(const char *program_name)
{
    printf("Usage: input.png [options]\n");
    printf("Options:\n");
    printf("  -h, --help              Show this help message and exit\n");
    printf("  -o, --output <file>     Specify the output file (default: out.png)\n");
    printf("  -i, --info              Print information about the PNG file\n");

    printf("\nCircle Options:\n");
    printf("  --circle                Enable circle drawing\n");
    printf("  --center <x.y>          Center coordinates of the circle\n");
    printf("  --radius <value>        Radius of the circle\n");
    printf("  --thickness <value>     Thickness of the circle line\n");
    printf("  --color <rrr.ggg.bbb> Color of the circle line\n");
    printf("  --fill                  Fill the circle\n");
    printf("  --fill_color <rrr.ggg.bbb> Color to fill the circle\n");

    printf("\nMirror Options:\n");
    printf("  --mirror                Enable mirroring\n");
    printf("  --axis <x|y>            Axis to mirror across (x or y)\n");
    printf("  --left_up <left.up>     Coordinates of the top-left corner of the area to mirror\n");
    printf("  --right_down <right.down> Coordinates of the bottom-right corner of the area to mirror\n");

    printf("\nCopy Options:\n");
    printf("  --copy                  Enable copying\n");
    printf("  --left_up <left.up>     Coordinates of the top-left corner of the source area\n");
    printf("  --right_down <right.down> Coordinates of the bottom-right corner of the source area\n");
    printf("  --dest_left_up <left.up>  Coordinates of the top-left corner of the destination area\n");
}

void print_png_info(png_structp png_ptr, png_infop info_ptr)
{
    png_uint_32 width, height;
    int bit_depth, color_type, interlace_type;

    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, &interlace_type, NULL, NULL);

    printf("PNG Info:\n");
    printf("  Width: %d\n", width);
    printf("  Height: %d\n", height);
    printf("  Bit Depth: %d\n", bit_depth);
    printf("  Color Type: %d\n", color_type);
    printf("  Interlace Type: %d\n", interlace_type);
}

bool is_png_file(const char *filename)
{
    FILE *fp = fopen(filename, "rb");
    if (!fp)
    {
        return false;
    }
    png_byte header[8];
    size_t bytes_read = fread(header, 1, 8, fp);
    fclose(fp);

    if (bytes_read != 8)
    {
        return false;
    }

    return png_sig_cmp(header, 0, 8) == 0;
}

void handle_error(const char *message)
{
    fprintf(stderr, "Error: %s\n", message);
}

png_bytep *read_png_file(const char *filename, int *width, int *height, png_byte *color_type, png_byte *bit_depth)
{
    FILE *fp = fopen(filename, "rb");
    if (!fp)
    {
        handle_error("File could not be opened for reading");
        return NULL;
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr)
    {
        handle_error("png_create_read_struct failed");
        fclose(fp);
        return NULL;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
        handle_error("png_create_info_struct failed");
        png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
        fclose(fp);
        return NULL;
    }

    if (setjmp(png_jmpbuf(png_ptr)))
    {
        handle_error("Error during png reading");
        png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
        fclose(fp);
        return NULL;
    }

    png_init_io(png_ptr, fp);
    png_read_info(png_ptr, info_ptr);

    *width = png_get_image_width(png_ptr, info_ptr);
    *height = png_get_image_height(png_ptr, info_ptr);
    *color_type = png_get_color_type(png_ptr, info_ptr);
    *bit_depth = png_get_bit_depth(png_ptr, info_ptr);

    if (*color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png_ptr);
    if (*color_type == PNG_COLOR_TYPE_GRAY && *bit_depth < 8)
        png_set_expand_gray_1_2_4_to_8(png_ptr);
    if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png_ptr);
    if (*bit_depth == 16)
        png_set_strip_16(png_ptr);
    if (*color_type == PNG_COLOR_TYPE_GRAY || *color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(png_ptr);

    png_read_update_info(png_ptr, info_ptr);

    int rowbytes = png_get_rowbytes(png_ptr, info_ptr);

    png_bytep *row_pointers = (png_bytep *)malloc(sizeof(png_bytep) * (*height));
    for (int y = 0; y < (*height); y++)
    {
        row_pointers[y] = (png_byte *)malloc(rowbytes);
    }

    png_read_image(png_ptr, row_pointers);

    fclose(fp);
    png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);

    return row_pointers;
}

void write_png_file(const char *filename, int width, int height, png_byte color_type, png_byte bit_depth, png_bytep *row_pointers)
{
    FILE *fp = fopen(filename, "wb");
    if (!fp)
    {
        handle_error("Could not open file %s for writing");
        return;
    }

    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr)
    {
        handle_error("png_create_write_struct failed");
        fclose(fp);
        return;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
        handle_error("png_create_info_struct failed");
        png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
        fclose(fp);
        return;
    }

    if (setjmp(png_jmpbuf(png_ptr)))
    {
        handle_error("Error during png writing");
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(fp);
        return;
    }

    png_init_io(png_ptr, fp);

    png_set_IHDR(png_ptr, info_ptr, width, height,
                 bit_depth, color_type, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    png_write_info(png_ptr, info_ptr);

    png_write_image(png_ptr, row_pointers);

    png_write_end(png_ptr, info_ptr);

    fclose(fp);
    png_destroy_write_struct(&png_ptr, &info_ptr);
}
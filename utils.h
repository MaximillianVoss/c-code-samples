#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <png.h>
#include "types.h"

void handle_error(const char *msg)
{
    fprintf(stderr, "Error: %s\n", msg);
}

bool is_png_file(const char *filename)
{
    FILE *fp = fopen(filename, "rb");
    if (!fp)
        return false;
    png_byte header[8];
    bool result = fread(header, 1, 8, fp) == 8 && png_sig_cmp(header, 0, 8) == 0;
    fclose(fp);
    return result;
}

bool read_png_file(const char *filename, PngImage *img)
{
    FILE *fp = fopen(filename, "rb");
    if (!fp)
        return false;

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!png_ptr || !info_ptr || setjmp(png_jmpbuf(png_ptr)))
    {
        fclose(fp);
        return false;
    }

    png_init_io(png_ptr, fp);
    png_read_info(png_ptr, info_ptr);

    img->width = png_get_image_width(png_ptr, info_ptr);
    img->height = png_get_image_height(png_ptr, info_ptr);
    img->color_type = png_get_color_type(png_ptr, info_ptr);
    img->bit_depth = png_get_bit_depth(png_ptr, info_ptr);

    png_read_update_info(png_ptr, info_ptr);
    img->rows = malloc(sizeof(png_bytep) * img->height);
    int rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    for (int y = 0; y < img->height; y++)
        img->rows[y] = malloc(rowbytes);

    png_read_image(png_ptr, img->rows);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);
    return true;
}

bool write_png_file(const char *filename, const PngImage *img)
{
    FILE *fp = fopen(filename, "wb");
    if (!fp)
        return false;

    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!png_ptr || !info_ptr || setjmp(png_jmpbuf(png_ptr)))
    {
        fclose(fp);
        return false;
    }

    png_init_io(png_ptr, fp);
    png_set_IHDR(png_ptr, info_ptr, img->width, img->height, img->bit_depth,
                 img->color_type, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    png_write_info(png_ptr, info_ptr);
    png_write_image(png_ptr, img->rows);
    png_write_end(png_ptr, NULL);
    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(fp);
    return true;
}

void print_png_info(const PngImage *img)
{
    printf("PNG Info:\n");
    printf("  Width: %d\n", img->width);
    printf("  Height: %d\n", img->height);
    printf("  Bit Depth: %d\n", img->bit_depth);
    printf("  Color Type: %d\n", img->color_type);
}

void free_png_image(PngImage *img)
{
    for (int y = 0; y < img->height; y++)
        free(img->rows[y]);
    free(img->rows);
}

// compare_pixels.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <png.h>

typedef struct
{
    int width, height;
    png_byte color_type, bit_depth;
    png_bytep *rows;
} PngImage;

int load_png(const char *filename, PngImage *img)
{
    FILE *fp = fopen(filename, "rb");
    if (!fp)
        return 0;

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!png_ptr || !info_ptr || setjmp(png_jmpbuf(png_ptr)))
    {
        fclose(fp);
        return 0;
    }

    png_init_io(png_ptr, fp);
    png_read_info(png_ptr, info_ptr);

    img->width = png_get_image_width(png_ptr, info_ptr);
    img->height = png_get_image_height(png_ptr, info_ptr);
    img->color_type = png_get_color_type(png_ptr, info_ptr);
    img->bit_depth = png_get_bit_depth(png_ptr, info_ptr);

    png_read_update_info(png_ptr, info_ptr);

    int rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    img->rows = malloc(sizeof(png_bytep) * img->height);
    for (int y = 0; y < img->height; y++)
        img->rows[y] = malloc(rowbytes);

    png_read_image(png_ptr, img->rows);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);
    return 1;
}

void free_image(PngImage *img)
{
    for (int y = 0; y < img->height; y++)
        free(img->rows[y]);
    free(img->rows);
}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s original.png saved.png\n", argv[0]);
        return 2;
    }

    PngImage a, b;
    if (!load_png(argv[1], &a) || !load_png(argv[2], &b))
    {
        fprintf(stderr, "❌ Ошибка загрузки одного из изображений.\n");
        return 2;
    }

    if (a.width != b.width || a.height != b.height || a.color_type != b.color_type || a.bit_depth != b.bit_depth)
    {
        printf("❌ Метаданные не совпадают\n");
        free_image(&a);
        free_image(&b);
        return 1;
    }

    int rowbytes = a.width * (a.bit_depth / 8) * (a.color_type == PNG_COLOR_TYPE_RGB ? 3 : 4);
    for (int y = 0; y < a.height; y++)
    {
        if (memcmp(a.rows[y], b.rows[y], rowbytes) != 0)
        {
            printf("❌ Пиксели отличаются в строке %d\n", y);
            free_image(&a);
            free_image(&b);
            return 1;
        }
    }

    printf("✅ Пиксели совпадают\n");
    free_image(&a);
    free_image(&b);
    return 0;
}

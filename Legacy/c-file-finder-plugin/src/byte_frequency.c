#include "byte_frequency.h"

#include <limits.h>
#include <stdio.h>

static int digit_value(char character)
{
    if (character >= '0' && character <= '9')
        return character - '0';
    if (character >= 'a' && character <= 'f')
        return character - 'a' + 10;
    if (character >= 'A' && character <= 'F')
        return character - 'A' + 10;
    return -1;
}

int byte_frequency_parse_value(const char *text, unsigned char *value)
{
    if (text == NULL || value == NULL)
        return BYTE_FREQUENCY_INVALID_ARGUMENT;

    unsigned int base = 10;
    size_t position = 0;
    if (text[0] == '0' && (text[1] == 'b' || text[1] == 'B'))
    {
        base = 2;
        position = 2;
    }
    else if (text[0] == '0' && (text[1] == 'x' || text[1] == 'X'))
    {
        base = 16;
        position = 2;
    }

    if (text[position] == '\0')
        return BYTE_FREQUENCY_INVALID_VALUE;

    unsigned int parsed = 0;
    for (; text[position] != '\0'; ++position)
    {
        int digit = digit_value(text[position]);
        if (digit < 0 || (unsigned int)digit >= base)
            return BYTE_FREQUENCY_INVALID_VALUE;

        parsed = parsed * base + (unsigned int)digit;
        if (parsed > UCHAR_MAX)
            return BYTE_FREQUENCY_OUT_OF_RANGE;
    }

    *value = (unsigned char)parsed;
    return BYTE_FREQUENCY_OK;
}

static int most_common_from_counts(
    const size_t counts[UCHAR_MAX + 1],
    size_t total,
    unsigned char *value)
{
    if (total == 0)
        return BYTE_FREQUENCY_EMPTY_INPUT;

    size_t maximum = 0;
    unsigned int result = 0;
    for (unsigned int current = 0; current <= UCHAR_MAX; ++current)
    {
        if (counts[current] > maximum)
        {
            maximum = counts[current];
            result = current;
        }
    }

    *value = (unsigned char)result;
    return BYTE_FREQUENCY_OK;
}

int byte_frequency_find_most_common(
    const unsigned char *data,
    size_t length,
    unsigned char *value)
{
    if (value == NULL || (data == NULL && length != 0))
        return BYTE_FREQUENCY_INVALID_ARGUMENT;

    size_t counts[UCHAR_MAX + 1] = {0};
    for (size_t index = 0; index < length; ++index)
        ++counts[data[index]];

    return most_common_from_counts(counts, length, value);
}

int byte_frequency_find_most_common_in_file(
    const char *path,
    unsigned char *value)
{
    if (path == NULL || value == NULL)
        return BYTE_FREQUENCY_INVALID_ARGUMENT;

    FILE *file = fopen(path, "rb");
    if (file == NULL)
        return BYTE_FREQUENCY_IO_ERROR;

    size_t counts[UCHAR_MAX + 1] = {0};
    size_t total = 0;
    unsigned char buffer[8192];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) != 0)
    {
        total += bytes_read;
        for (size_t index = 0; index < bytes_read; ++index)
            ++counts[buffer[index]];
    }

    int result = ferror(file)
        ? BYTE_FREQUENCY_IO_ERROR
        : most_common_from_counts(counts, total, value);
    fclose(file);
    return result;
}

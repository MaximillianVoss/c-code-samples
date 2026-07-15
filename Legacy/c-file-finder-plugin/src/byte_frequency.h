#ifndef BYTE_FREQUENCY_H
#define BYTE_FREQUENCY_H

#include <stddef.h>

enum byte_frequency_status
{
    BYTE_FREQUENCY_OK = 0,
    BYTE_FREQUENCY_INVALID_ARGUMENT = -1,
    BYTE_FREQUENCY_INVALID_VALUE = -2,
    BYTE_FREQUENCY_OUT_OF_RANGE = -3,
    BYTE_FREQUENCY_EMPTY_INPUT = -4,
    BYTE_FREQUENCY_IO_ERROR = -5
};

/* Accepts decimal, 0b-prefixed binary, and 0x-prefixed hexadecimal values. */
int byte_frequency_parse_value(const char *text, unsigned char *value);

/* Returns the lowest byte value when several values have the same frequency. */
int byte_frequency_find_most_common(
    const unsigned char *data,
    size_t length,
    unsigned char *value);

int byte_frequency_find_most_common_in_file(
    const char *path,
    unsigned char *value);

#endif

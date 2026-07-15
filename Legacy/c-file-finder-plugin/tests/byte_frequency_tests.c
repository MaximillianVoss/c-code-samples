#include "byte_frequency.h"

#include <stdio.h>

#define CHECK(condition)                                                        \
    do                                                                          \
    {                                                                           \
        if (!(condition))                                                       \
        {                                                                       \
            fprintf(stderr, "check failed at %s:%d: %s\n",                    \
                __FILE__, __LINE__, #condition);                                \
            return 1;                                                           \
        }                                                                       \
    } while (0)

static int test_value_parsing(void)
{
    unsigned char value = 0;
    CHECK(byte_frequency_parse_value("255", &value) == BYTE_FREQUENCY_OK);
    CHECK(value == 255);
    CHECK(byte_frequency_parse_value("0xff", &value) == BYTE_FREQUENCY_OK);
    CHECK(value == 255);
    CHECK(byte_frequency_parse_value("0b11111111", &value) == BYTE_FREQUENCY_OK);
    CHECK(value == 255);
    CHECK(byte_frequency_parse_value("", &value) == BYTE_FREQUENCY_INVALID_VALUE);
    CHECK(byte_frequency_parse_value("0x", &value) == BYTE_FREQUENCY_INVALID_VALUE);
    CHECK(byte_frequency_parse_value("0b2", &value) == BYTE_FREQUENCY_INVALID_VALUE);
    CHECK(byte_frequency_parse_value("256", &value) == BYTE_FREQUENCY_OUT_OF_RANGE);
    return 0;
}

static int test_frequency_counting(void)
{
    const unsigned char data[] = {255, 1, 255, 2, 255, 1};
    unsigned char value = 0;
    CHECK(byte_frequency_find_most_common(data, sizeof(data), &value) == BYTE_FREQUENCY_OK);
    CHECK(value == 255);

    const unsigned char tie[] = {4, 9, 9, 4};
    CHECK(byte_frequency_find_most_common(tie, sizeof(tie), &value) == BYTE_FREQUENCY_OK);
    CHECK(value == 4);
    CHECK(byte_frequency_find_most_common(NULL, 0, &value) == BYTE_FREQUENCY_EMPTY_INPUT);
    return 0;
}

int main(void)
{
    if (test_value_parsing() != 0 || test_frequency_counting() != 0)
        return 1;

    puts("byte_frequency_tests: ok");
    return 0;
}

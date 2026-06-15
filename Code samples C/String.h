#pragma once
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma region Type String

typedef struct
{
	char* symbols;
	size_t length;
} String;

// Header-only sample API: functions are static to avoid duplicate symbols when
// the same helper header is included from several .c files in one project.
static String StringInit(size_t capacity)
{
	String result;
	result.symbols = (char*)calloc(capacity + 1, sizeof(char));
	result.length = 0;
	return result;
}

// Frees owned storage. The value is passed by copy, so callers should not use
// str.symbols after this call unless they reset their local String instance.
static void StringDelete(String str)
{
	free(str.symbols);
}

static size_t StringGetLength(const char* str)
{
	return str == NULL ? 0 : strlen(str);
}

// Creates an owning copy. On allocation failure returns { NULL, 0 }.
static String StringCopy(const char* str)
{
	String result = StringInit(StringGetLength(str));
	if (str != NULL && result.symbols != NULL)
	{
		result.length = strlen(str);
		memcpy(result.symbols, str, result.length + 1);
	}
	return result;
}

static String StringFromBoolean(bool value)
{
	return value ? StringCopy("true") : StringCopy("false");
}

static int StringCompare(String str1, String str2)
{
	int compareResult = strcmp(str1.symbols == NULL ? "" : str1.symbols, str2.symbols == NULL ? "" : str2.symbols);
	if (compareResult > 0)
	{
		return 1;
	}
	if (compareResult < 0)
	{
		return -1;
	}
	return 0;
}

// Appends one byte character. If allocation fails, the original String remains
// valid and unchanged.
static void StringAdd(String* str, char c)
{
	if (str == NULL)
	{
		return;
	}

	char* result = (char*)realloc(str->symbols, (str->length + 2) * sizeof(char));
	if (result == NULL)
	{
		return;
	}

	str->symbols = result;
	str->symbols[str->length] = c;
	str->length++;
	str->symbols[str->length] = '\0';
}

static int StringGetCharIndex(const char* str, char c)
{
	if (str != NULL)
	{
		for (size_t i = 0; i < strlen(str); i++)
		{
			if (str[i] == c)
			{
				return (int)i;
			}
		}
	}
	return -1;
}

static bool IsStringSimpleContains(const char* str, char c)
{
	if (str == NULL)
	{
		return false;
	}

	for (size_t i = 0; i < strlen(str); i++)
	{
		if (str[i] == c)
		{
			return true;
		}
	}
	return false;
}

static bool IsStringContains(const String* str, char c)
{
	if (str == NULL)
	{
		return false;
	}
	return IsStringSimpleContains(str->symbols, c);
}

static bool StringIsDigit(char c)
{
	return c >= '0' && c <= '9';
}

// Accepts signed decimal numbers with optional '.' or ',' fractional separator.
// Leading zeroes are rejected for multi-digit integer parts, but "0" and
// fractions like "0.5" / "-0,5" are valid.
static bool StringIsNumber(const String* str)
{
	if (str == NULL || str->symbols == NULL || str->length == 0)
	{
		return false;
	}

	size_t index = 0;
	if (str->symbols[index] == '+' || str->symbols[index] == '-')
	{
		index++;
		if (index == str->length)
		{
			return false;
		}
	}

	size_t integerStart = index;
	size_t integerDigits = 0;
	while (index < str->length && StringIsDigit(str->symbols[index]))
	{
		integerDigits++;
		index++;
	}

	if (integerDigits == 0)
	{
		return false;
	}

	bool hasFraction = false;
	if (index < str->length && (str->symbols[index] == '.' || str->symbols[index] == ','))
	{
		hasFraction = true;
		index++;
		size_t fractionDigits = 0;
		while (index < str->length && StringIsDigit(str->symbols[index]))
		{
			fractionDigits++;
			index++;
		}
		if (fractionDigits == 0)
		{
			return false;
		}
	}

	if (index != str->length)
	{
		return false;
	}

	if (integerDigits > 1 && str->symbols[integerStart] == '0')
	{
		return false;
	}

	return integerDigits > 0 || hasFraction;
}

#pragma endregion

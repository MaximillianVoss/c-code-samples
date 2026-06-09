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

String StringInit(size_t capacity)
{
	String result;
	result.symbols = (char*)calloc(capacity + 1, sizeof(char));
	result.length = 0;
	return result;
}

void StringDelete(String str)
{
	free(str.symbols);
}

size_t StringGetLength(const char* str)
{
	return str == NULL ? 0 : strlen(str);
}

String StringCopy(const char* str)
{
	String result = StringInit(StringGetLength(str));
	if (str != NULL && result.symbols != NULL)
	{
		result.length = strlen(str);
		memcpy(result.symbols, str, result.length + 1);
	}
	return result;
}

String StringFromBoolean(bool value)
{
	return value ? StringCopy("true") : StringCopy("false");
}

int StringCompare(String str1, String str2)
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

void StringAdd(String* str, char c)
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

int StringGetCharIndex(const char* str, char c)
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

bool IsStringSimpleContains(const char* str, char c)
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

bool IsStringContains(const String* str, char c)
{
	if (str == NULL)
	{
		return false;
	}
	return IsStringSimpleContains(str->symbols, c);
}

bool StringIsNumber(const String* str)
{
	if (str == NULL || str->symbols == NULL || str->length == 0)
	{
		return false;
	}

	const char* signs = "+-";
	const char* digits = "1234567890";
	const char* delimiters = ".,";
	const char* alphabet = "+-.,1234567890";
	int delimitersCount = 0;
	int signsCount = 0;
	int digitCount = 0;

	for (size_t i = 0; i < str->length; i++)
	{
		char currentChar = str->symbols[i];

		if (IsStringSimpleContains(signs, currentChar))
		{
			signsCount++;
			if (i != 0 || digitCount > 0 || signsCount > 1)
			{
				return false;
			}
		}

		if (IsStringSimpleContains(delimiters, currentChar))
		{
			delimitersCount++;
			if (delimitersCount > 1 || i == 0 || digitCount < 1 || i == str->length - 1)
			{
				return false;
			}
		}

		if (currentChar == '0' && digitCount == 0 && str->length > 1)
		{
			return false;
		}
		if (IsStringSimpleContains(digits, currentChar))
		{
			digitCount++;
		}

		if (!IsStringSimpleContains(alphabet, currentChar))
		{
			return false;
		}
	}

	if ((signsCount > 0 || delimitersCount > 0) && digitCount == 0)
	{
		return false;
	}

	return true;
}

#pragma endregion

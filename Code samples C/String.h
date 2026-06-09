#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#pragma region Type String

typedef struct
{
	char* symbols;
	size_t length;
} String;

String StringInit(size_t length)
{
	String result;
	result.symbols = (char*)calloc(length, sizeof(char));
	result.length = length;
	return result;
}

void StringDelete(String str)
{
	free(str.symbols);
}

int StringGetLength(char* str)
{
	if (str == NULL)
		return 0;
	int length = 0;
	for (int i = 0;; i++)
	{
		if (str[i] == '\0')
			break;
		length++;
	}
	return length;
}

String StringCopy(char* str)
{
	String result;
	result.length = StringGetLength(str);
	result.symbols = (char*)calloc(result.length + 1, sizeof(char));
	for (int i = 0; i < result.length; i++)
	{
		result.symbols[i] = str[i];
	}
	return result;
}

String StringFromBoolean(bool b) {
	return b == true ? StringCopy("true") : StringCopy("false");
}

int StringCompare(String str1, String str2)
{
	if (str1.length > str2.length)
	{
		return 1;
	}
	else if (str1.length < str2.length)
	{
		return -1;
	}
	else
	{
		for (int i = 0; i < str1.length; i++)
		{
			if (str1.symbols[i] > str2.symbols[i])
			{
				return 1;
			}
			else if (str1.symbols[i] < str2.symbols[i])
			{
				return -1;
			}
		}
		return 0;
	}
}

void StringAdd(String* str, char c)
{
	if (str->length == 0)
	{
		StringDelete(*str);
		str->symbols = (char*)calloc(2, sizeof(char));
		str->length += 2;
	}
	else
	{
		str->symbols = (char*)realloc(str->symbols, (++(str->length)) * sizeof(char));
	}
	str->symbols[str->length - 2] = c;
	str->symbols[str->length - 1] = '\0';
}

int StringGetCharIndex(char* str, char c) {
	if (str != NULL) {
		for (size_t i = 0; i < strlen(str); i++)
			if (str[i] == c)
				return i;
	}
	return -1;
}

bool IsStringSimpleContains(char* str, char c) {
	if (str == NULL)
		return false;
	for (size_t i = 0; i < strlen(str); i++)
	{
		if (str[i] == c)
			return true;
	}
	return false;
}

bool IsStringContains(String* str, char c) {
	if (str == NULL)
		return false;
	return IsStringSimpleContains(str->symbols, c);
}

bool StringIsNumber(String* str) {
	if (str == NULL)
		return false;
	const char* signs = "+-";
	const char* digits = "1234567890";
	const char* delimiters = ".,";
	const char* alphabet = "+-.,1234567890";
	int delimitersCount = 0;
	int signsCount = 0;
	int digitCount = 0;
	for (size_t i = 0; i < StringGetLength(str->symbols); i++)
	{
		char currentChar = str->symbols[i];
#pragma region Знаки
		if (IsStringSimpleContains(signs, currentChar))
		{
			signsCount++;
			if (i != 0)
				return false;
			if (digitCount > 0)
				return false;
			if (signsCount > 1)
				return false;
		}
#pragma endregion

#pragma region Разделители
		if (IsStringSimpleContains(delimiters, currentChar))
		{
			delimitersCount++;
			if (delimitersCount > 1)
				return false;
			if (i == 0)
				return false;
			if (digitCount < 1)
				return false;
			if (i == StringGetLength(str->symbols) - 1)
				return false;
		}
#pragma endregion

#pragma region Цифры
		if (currentChar == '0' && digitCount == 0)
			return false;
		if (IsStringSimpleContains(digits, str->symbols[i])) {
			digitCount++;
		}
#pragma endregion

		if (!IsStringSimpleContains(alphabet, str->symbols[i]))
			return false;
	}
	if ((signsCount > 0 || delimitersCount > 0) && digitCount == 0)
		return false;
	return true;
}

#pragma endregion
#pragma once
#include "stdafx.h"

char* StrAppendChar(char* str, char c);
char* StrAppendStr(char* str1, const char* str2);

char* StrAppendChar(char* str, char c)
{
	size_t length = str != NULL ? strlen(str) : 0;
	char* result = (char*)realloc(str, (length + 2) * sizeof(char));
	if (result == NULL)
	{
		return str;
	}

	result[length] = c;
	result[length + 1] = '\0';
	return result;
}

char* StrAppendStr(char* str1, const char* str2)
{
	if (str2 == NULL)
	{
		return str1;
	}

	for (size_t i = 0; i < strlen(str2); i++)
	{
		str1 = StrAppendChar(str1, str2[i]);
	}

	return str1;
}

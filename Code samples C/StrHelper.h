#pragma once
#include "stdafx.h"

// Returns the possibly reallocated pointer. On allocation failure the original
// pointer is returned unchanged, so ownership always stays with the caller.
static char* StrAppendChar(char* str, char c)
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

static char* StrAppendStr(char* str1, const char* str2)
{
	if (str2 == NULL || str2[0] == '\0')
	{
		return str1;
	}

	size_t length1 = str1 != NULL ? strlen(str1) : 0;
	size_t length2 = strlen(str2);
	char* result = (char*)realloc(str1, (length1 + length2 + 1) * sizeof(char));
	if (result == NULL)
	{
		return str1;
	}

	memcpy(result + length1, str2, length2 + 1);
	return result;
}

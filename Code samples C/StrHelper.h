#pragma once
#include "stdafx.h"
char * StrAppendChar(char *str, char c);
char * StrAppendStr(char *str1, char *str2);

char * StrAppendChar(char *str, char c) {
	if (str != NULL) {
		int length = strlen(str);
		str = (char*)realloc(str, (length + 2) * sizeof(char));
		str[length] = c;
		str[length + 1] = '\0';
	}
	return str;
}

char * StrAppendStr(char *str1, char *str2) {
	for (int i = 0; i < strlen(str2); i++)
		str1 = StrAppendChar(str1, str2[i]);
	return str1;
}
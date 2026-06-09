#pragma once
#include "stdafx.h"

char* ConvertIntToStr(int value)
{
	int length = _scprintf("%d", value);
	char* result = (char*)calloc((size_t)length + 1, sizeof(char));
	if (result != NULL)
	{
		sprintf(result, "%d", value);
	}
	return result;
}

char* ConvertFloatToStr(float value)
{
	int length = _scprintf("%g", value);
	char* result = (char*)calloc((size_t)length + 1, sizeof(char));
	if (result != NULL)
	{
		sprintf(result, "%g", value);
	}
	return result;
}

char* ConvertDoubleToStr(double value)
{
	int length = _scprintf("%g", value);
	char* result = (char*)calloc((size_t)length + 1, sizeof(char));
	if (result != NULL)
	{
		sprintf(result, "%g", value);
	}
	return result;
}

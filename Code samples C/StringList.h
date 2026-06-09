#pragma once
#include "String.h"

#pragma region Type StringList

typedef struct
{
	String* items;
	size_t length;
} StringList;

StringList StringListInit()
{
	StringList list;
	list.items = NULL;
	list.length = 0;
	return list;
}

void StringListDelete(StringList* list)
{
	if (list == NULL)
	{
		return;
	}

	for (size_t i = 0; i < list->length; i++)
	{
		StringDelete(list->items[i]);
	}
	free(list->items);
	list->items = NULL;
	list->length = 0;
}

void StringListAdd(StringList* list, String str)
{
	if (list == NULL)
	{
		return;
	}

	String* result = (String*)realloc(list->items, (list->length + 1) * sizeof(String));
	if (result == NULL)
	{
		return;
	}

	list->items = result;
	list->items[list->length] = str;
	list->length++;
}

void StringListPrint(StringList list, char delimiter)
{
	for (size_t i = 0; i < list.length; i++)
	{
		printf("%s", list.items[i].symbols);
		printf("%c", delimiter);
	}
}

void StringListSet(StringList* list, String str, size_t index)
{
	if (list != NULL && index < list->length)
	{
		StringDelete(list->items[index]);
		list->items[index] = StringCopy(str.symbols);
	}
}

void StringListDeleteAt(StringList* list, size_t index)
{
	if (list == NULL || index >= list->length)
	{
		return;
	}

	StringDelete(list->items[index]);
	for (size_t i = index; i + 1 < list->length; i++)
	{
		list->items[i] = list->items[i + 1];
	}

	list->length--;
	if (list->length == 0)
	{
		free(list->items);
		list->items = NULL;
		return;
	}

	String* result = (String*)realloc(list->items, list->length * sizeof(String));
	if (result != NULL)
	{
		list->items = result;
	}
}

void StringListSort(StringList* list, bool isAscending)
{
	if (list == NULL)
	{
		return;
	}

	for (size_t i = 0; i + 1 < list->length; i++)
	{
		for (size_t j = 0; j + 1 < list->length - i; j++)
		{
			int compareResult = isAscending
				? StringCompare(list->items[j], list->items[j + 1])
				: StringCompare(list->items[j + 1], list->items[j]);
			if (compareResult > 0)
			{
				String buffer = list->items[j];
				list->items[j] = list->items[j + 1];
				list->items[j + 1] = buffer;
			}
		}
	}
}

StringList SplitStringToStringList(String str)
{
	StringList result = StringListInit();
	String buffer = StringInit(0);

	for (size_t i = 0; i < str.length; i++)
	{
		if (str.symbols[i] != ' ')
		{
			StringAdd(&buffer, str.symbols[i]);
		}
		else if (buffer.length != 0)
		{
			StringListAdd(&result, StringCopy(buffer.symbols));
			StringDelete(buffer);
			buffer = StringInit(0);
		}
	}

	if (buffer.length != 0)
	{
		StringListAdd(&result, StringCopy(buffer.symbols));
	}
	StringDelete(buffer);

	return result;
}

StringList SplitStirngToStringList(String str)
{
	return SplitStringToStringList(str);
}

#pragma endregion

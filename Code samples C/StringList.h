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
	for (int i = 0; i < list->length; i++)
	{
		StringDelete(list->items[i]);
	}
	free(list->items);
}

void StringListAdd(StringList* list, String str)
{
	if (list->length == 0)
		list->items = (String*)malloc(++(list->length) * sizeof(String));
	else
		list->items = (String*)realloc(list->items, ++(list->length) * sizeof(String));
	list->items[list->length - 1] = str;
}

void StringListPrint(StringList list, char delimeter)
{
	for (int i = 0; i < list.length; i++)
	{
		printf("%s", list.items[i].symbols);
		printf("%c", delimeter);
	}
}

void StringListSet(StringList* list, String str, int index)
{
	if (index >= 0 && index < list->length)
	{
		StringDelete(list->items[index]);
		list->items[index] = StringCopy(str.symbols);
	}
}

void StringListDeleteAt(StringList* list, int index)
{
	if (index >= 0 && index < list->length)
	{
		for (int i = index; i < list->length - 1; i++)
			StringListSet(list, list->items[i + 1], i);
		StringDelete(list->items[list->length - 1]);
		list->items = (String*)realloc(list->items, --(list->length) * sizeof(String));
	}
}

void StringListSort(StringList* list, bool flag)
{
#pragma region oldvariant

	// int help = 1;
	// for (int i = 0; i < list->length - help; i++)
	// {
	//     if (list->items[i].length > list->items[i + 1].length)
	//     {
	//         String str = StringCopy(list->items[i].symbols);
	//         list->items[i] = StringCopy(list->items[i + 1].symbols);
	//         list->items[i + 1] = StringCopy(str.symbols);
	//         StringDelete(str);
	//         PrintList(*list);
	//         printf("\n");
	//     }
	//     if (i == list->length - help - 1)
	//     {
	//         i = 0;
	//         help++;
	//     }
	// }
#pragma endregion

	for (int i = 0; i < list->length - 1; i++)
	{
		for (int j = 0; j < list->length - i - 1; j++)
		{
			int isChange = 0;
			if (flag)
				isChange = StringCompare(list->items[j], list->items[j + 1]);
			else
				isChange = StringCompare(list->items[j + 1], list->items[j]);
			if (isChange > 0)
			{
				String str = StringCopy(list->items[j].symbols);
				StringDelete(list->items[j]);
				list->items[j] = StringCopy(list->items[j + 1].symbols);
				StringDelete(list->items[j + 1]);
				list->items[j + 1] = StringCopy(str.symbols);
				StringDelete(str);
			}
		}
	}
}

StringList SplitStirngToStringList(String str)
{
	// 123 456 678
	StringList result = StringListInit();
	String buffer = StringInit(0);
	for (int i = 0; i < str.length; i++)
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
		StringDelete(buffer);
	}
	else
	{
		StringDelete(buffer);
	}

	return result;
}

#pragma endregion

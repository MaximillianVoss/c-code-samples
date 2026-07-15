#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>

#pragma region Вспомогательные структуры

#pragma region Строка

#pragma region Объявление структуры
struct String
{
    char *items;
    size_t length;
};
#pragma endregion

#pragma region Методы
struct String InitString(char *str)
{
    struct String string;
    string.length = strlen(str);
    string.items = (char *)malloc((string.length + 1) * sizeof(char));
    strcpy(string.items, str);
    string.items[string.length] = '\0';
    return string;
}
void PrintString(struct String *str)
{
    printf("%s", str->items);
}
void DeleteString(struct String *str)
{
    if (str != NULL && str->items != NULL)
        free(str->items);
}
#pragma endregion

#pragma endregion

#pragma region Список строк

#pragma region Объявление структуры
struct StringList
{
    struct String *stirngs;
    int count;
};
#pragma endregion

#pragma region Методы
struct StringList InitStringList()
{
    struct StringList list;
    list.count = 0;
    list.stirngs = NULL;
    return list;
}
void AddToStringList(struct StringList *list, char *str)
{
    if (list != NULL)
    {
        list->stirngs = (struct String *)realloc(list->stirngs, ++list->count * sizeof(struct String));
        list->stirngs[list->count - 1] = InitString(str);
    }
}
void PrintStringList(struct StringList list)
{
    if (list.stirngs != NULL)
    {
        for (int i = 0; i < list.count; i++)
        {
            PrintString(&list.stirngs[i]);
            printf("\n");
        }
    }
}
void DeleteStringList(struct StringList *list)
{
    if (list != NULL && list->stirngs != NULL)
    {
        for (int i = 0; i < list->count; i++)
            DeleteString(&list->stirngs[i]);
        free(list->stirngs);
    }
}

#pragma endregion

#pragma endregion

#pragma endregion

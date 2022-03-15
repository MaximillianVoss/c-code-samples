#include "plugin_api.h"
#include "structures.h"
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#pragma region Массив поддерживаемых опций
static struct plugin_option g_po_arr[] = {
    /*
        struct plugin_option {
            struct option {
               const char *name;
               int         has_arg;
               int        *flag;
               int         val;
            } opt,
            char *opt_descr
        }
    */
    {
        {
            "freq-byte",
            required_argument,
            0,
            0,
        },
        "Поиск файлов, в которых заданный байт — самый частый.Пример использования: '--freq-byte 0xfe' или '--freq-byte 0b1010' или '--freq-byte 254'"},
};
#pragma endregion

#pragma region Методы

#pragma region Проверки
bool IsStrDec(const char *str)
{
    if (str == NULL)
        return false;
    char *alphabet = "1234567890";
    for (size_t i = 0; i < strlen(str); i++)
    {
        char *strPointer = strchr(alphabet, str[i]);
        if (strPointer == NULL)
        {
            free(strPointer);
            return false;
        }
    }
    return true;
}
bool IsStrBin(const char *str)
{
    if (str == NULL)
        return false;
    char *alphabet = "01";
    if (strlen(str) < 2)
        return false;
    if (str[0] != '0' || str[1] != 'b')
        return false;
    for (size_t i = 2; i < strlen(str); i++)
    {
        char *strPointer = strchr(alphabet, str[i]);
        if (strPointer == NULL)
        {
            free(strPointer);
            return false;
        }
    }
    return true;
}
bool IsStrHex(const char *str)
{
    if (str == NULL)
        return false;
    char *alphabet = "0123456789abcdefABCDEF";
    if (strlen(str) < 2)
        return false;
    if (str[0] != '0' || str[1] != 'x')
        return false;
    for (size_t i = 2; i < strlen(str); i++)
    {
        char *strPointer = strchr(alphabet, str[i]);
        if (strPointer == NULL)
        {
            free(strPointer);
            return false;
        }
    }
    return true;
}
#pragma endregion

#pragma region Конвертация
int DecStrToDec(const char *str)
{
    assert(IsStrDec(str) == true);
    return strtol(str, NULL, 10);
}
int BinStrToDec(const char *str)
{
    assert(IsStrBin(str) == true);
    return strtol(&str[2], NULL, 2);
}
int HexStrToDec(const char *str)
{
    assert(IsStrHex(str) == true);
    return strtol(str, NULL, 16);
}
#pragma endregion

#pragma region Поиск файлов с указанным байтом
int GetMostByteFromFile(char *path)
{
    char *fileBytes = NULL;
    long fileBytesCount = 0;
#pragma region Считывание байтов файла
    FILE *file = fopen(path, "rb");
    if (file == NULL)
    {
        perror("Cannot open file!");
        return -1;
    }
    fseek(file, 0, SEEK_END);
    fileBytesCount = ftell(file);
    rewind(file);
    fileBytes = (char *)malloc(fileBytesCount * sizeof(char));
    size_t resultFread = fread(fileBytes, 1, fileBytesCount, file);
    if ((long)resultFread != fileBytesCount)
    {
        fputs("Ошибка чтения\n", stderr);
        printf("Число считанныйх байт:%li число байт в файле:%li\n", resultFread, fileBytesCount);
        exit(3);
    }
    fclose(file);
#pragma endregion

    int differentBytesCount = 255;
#pragma region Нахождение самого частого байта
    int *bytesFrequency = (int *)malloc(differentBytesCount * sizeof(int));
    for (int i = 0; i < differentBytesCount; i++)
        bytesFrequency[i] = 0;
    for (int i = 0; i < fileBytesCount; i++)
        bytesFrequency[(int)fileBytes[i]]++;
    int max = 0;
    int maxIndex = 0;
    for (int i = 0; i < differentBytesCount; i++)
        if (bytesFrequency[i] > max)
        {
            max = bytesFrequency[i];
            maxIndex = i;
        }
#pragma endregion
    free(bytesFrequency);
    free(fileBytes);
    return maxIndex;
}
struct StringList GetFilesWithIndent(const char *folderPath, int byteValue, int indent)
{
    DIR *dir;
    struct dirent *entry;
    struct StringList list = InitStringList();
    if (!(dir = opendir(folderPath)))
    {
        if (LAB1DEBUG)
            perror("Не удалось найти папку!");
        return list;
    }
    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_type == DT_DIR)
        {
            char path[1024];
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            snprintf(path, sizeof(path), "%s/%s", folderPath, entry->d_name);
            if (LAB1DEBUG == true)
                printf("%*s[%s]\n", indent, "", entry->d_name);
            struct StringList subFolderList = GetFilesWithIndent(path, byteValue, indent + 2);
            for (int i = 0; i < subFolderList.count; i++)
                AddToStringList(&list, subFolderList.stirngs[i].items);
            DeleteStringList(&subFolderList);
        }
        else
        {
            if (LAB1DEBUG == true)
                printf("%*s- %s\n", indent, "", entry->d_name);
            char path[1024];
            snprintf(path, sizeof(path), "%s/%s", folderPath, entry->d_name);
            if (GetMostByteFromFile(path) == byteValue)
                AddToStringList(&list, entry->d_name);
        }
    }
    closedir(dir);
    free(entry);
    return list;
}
struct StringList GetFiles(const char *folderPath, int byteValue)
{
    return GetFilesWithIndent(folderPath, byteValue, 4);
}
#pragma endregion

#pragma region методы API
int plugin_get_info(struct plugin_info *ppi)
{
    if (!ppi)
    {
        fprintf(stderr, "Ошибка: некорректное значение plugin_info!\n");
        return -1;
    }

    ppi->plugin_purpose = "Поиск файлов, в которых заданный байт — самый частый.\nВерсия:1.0.0\nНомер варианта: 4";
    ppi->plugin_author = "Кондакова Карина Андреевна, N3250";
    ppi->sup_opts_len = 1;
    ppi->sup_opts = g_po_arr;
    return 0;
}

int plugin_process_file(const char *fname,
                        struct option in_opts[],
                        size_t in_opts_len)
{
    if (in_opts_len != 1)
        return -1;
    else
    {
        if (LAB1DEBUG)
        {
            printf("Имя файла/папки:%s\n", fname);
            printf("Переданные пагину 'libkakN3250' параметры:\n");
            for (size_t i = 0; i < in_opts_len; i++)
                printf("%s %s\n", in_opts[0].name, (char *)in_opts[0].flag);
        }
        if (strcmp(in_opts[0].name, "freq-byte") == 0)
        {
            if (LAB1DEBUG == true)
                printf("Началась обработка файлов по указанному пути!\n");
            bool isConverted = false;
            char *byteValueStr = (char *)in_opts[0].flag;
            int byteValue = -1;
            if (IsStrDec(byteValueStr))
            {
                isConverted = true;
                byteValue = DecStrToDec(byteValueStr);
            }
            if (IsStrBin(byteValueStr))
            {
                isConverted = true;
                byteValue = BinStrToDec(byteValueStr);
            }
            if (IsStrHex(byteValueStr))
            {
                isConverted = true;
                byteValue = HexStrToDec(byteValueStr);
            }
            if (isConverted == false)
            {
                printf("Указан некорректный параметр для байта!");
                return -1;
            }
            if (LAB1DEBUG == true)
                printf("Десятичное значение искомого байта:%i\n", byteValue);
            struct StringList filesList = GetFiles(fname, byteValue);
            // printf("Были найдены следующие файлы:\n");
            PrintStringList(filesList);
            DeleteStringList(&filesList);
        }
        else
            return -1;
        return 0;
    }
}
#pragma endregion

#pragma endregion
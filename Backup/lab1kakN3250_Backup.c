// Call plugin_process_file() from given .so
//
// Compile with:
// gcc -o lab1call lab1call.c -ldl
//
// (c) Alexei Guirik, 2021
// This source is licensed under CC BY-NC 4.0
// (https://creativecommons.org/licenses/by-nc/4.0/)
//

#include "plugin_api.h"
#include "structures.h"
#include <dlfcn.h>
#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma region ИМЯ

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

#pragma region Распозначение опций
// -P dir Каталог с плагинами.
// -A Объединение опций плагинов с помощью операции «И» (действует по умолчанию).
// -O Объединение опций плагинов с помощью операции «ИЛИ».
// -N Инвертирование условия поиска(после объединения опций плагинов с помощью - A или - O).
// -v Вывод версии программы и информации о программе(ФИО исполнителя, номер группы, номер варианта лабораторной).
// -h Вывод справки по опциям
// --freq-byte <значение> <путь к каталогу с файлами>
void PrintArgs(char *argv[], int argc)
{
    printf("Количество переданных аргументов:%i\n", argc);
    if (argc > 0)
    {
        printf("Список аргументов:\n");
        for (int i = 0; i < argc; i++)
            printf("%s\n", argv[i]);
    }
    else
        printf("Аргументов не передано!");
}
void SelectOption(char *argv[], int argc)
{
    if (LAB1DEBUG == true)
        PrintArgs(argv, argc);
    if (argc == 1)
        return;
    if (argc == 5)
    {
        if (strcmp("--freq-byte", argv[2]) == 0)
        {
            char *parsedParam = argv[3];
            bool isConverted = false;
            int byteValue = -1;
            if (IsStrDec(parsedParam))
            {
                isConverted = true;
                byteValue = DecStrToDec(parsedParam);
            }
            if (IsStrBin(parsedParam))
            {
                isConverted = true;
                byteValue = BinStrToDec(parsedParam);
            }
            if (IsStrHex(parsedParam))
            {
                isConverted = true;
                byteValue = HexStrToDec(parsedParam);
            }
            if (isConverted == false)
            {
                printf("Указан некорректный параметр для байта!");
                return;
            }
            if (LAB1DEBUG == true)
                printf("Искомый байт:%i\n", byteValue);
            // struct StringList filesList = GetFiles(argv[3], byteValue);
            // printf("Были найдены следующие файлы:\n");
            // PrintStringList(filesList);
            // DeleteStringList(&filesList);
        }
    }
    if (argc == 2)
    {
        if (strcmp("-h", argv[1]) == 0)
        {
            printf("Доступные комманды:\n");
            printf("-P dir Каталог с плагинами.\n");
            printf("-A Объединение опций плагинов с помощью операции «И» (действует по умолчанию).\n");
            printf("-O Объединение опций плагинов с помощью операции «ИЛИ».\n");
            printf("-N Инвертирование условия поиска(после объединения опций плагинов с помощью - A или - O).\n");
            printf("-v Вывод версии программы и информации о программе(ФИО исполнителя, номер группы, номер варианта лабораторной).\n");
            printf("-h Вывод справки по опциям\n");
            printf("--freq-byte <значение> <путь к каталогу с файлами> (значение может задаваться в либо в двоичной(0b...),либо в десятичной, либо шестнадцатеричной(0x...) системах.)\n ");
        }
        if (strcmp("-v", argv[1]) == 0)
        {
            printf("Информация о программе:\n");
            printf("Поиска файлов, в которых заданный байт — самый частый.\n");
            printf("Версия:1.0.0\n");
            printf("Кондакова Кария Андреевна\n");
            printf("N3250\n");
            printf("Номер варианта: 4\n");
        }
    }
}
#pragma endregion

#pragma region Проверка плагина

bool CheckPlugin(char *argv[], int argc)
{
    if (argc > 1)
    {
        // Флаг прохождения всех проверок
        bool isPassed = true;
        // Имя библиотеки, должно быть вторым аргуметом
        char *libPath = strdup(argv[1]);
        // Информация о плагине
        struct plugin_info pluginInfo = {0};
        // // Name of the file to analyze. Should be passed as the last argumtent.
        // char *file_name = strdup(argv[argc - 1]);

        int opts_to_pass_len = 0;
        struct option *opts_to_pass = NULL;
        struct option *longopts = NULL;

#pragma region Вывод переданных аргументов

        if (LAB1DEBUG == true)
            PrintArgs(argv, argc);
#pragma endregion

#pragma region Проверка библиотеки на наличие
        void *libPointer = dlopen(libPath, RTLD_LAZY);
        if (!libPointer && isPassed)
        {
            fprintf(stderr, "ОШИБКА: не удалось открыть файл с плагином: %s\n", dlerror());
            goto CheckPluginEND;
        }
#pragma endregion

#pragma region Проверка наличия и работоспособности функции "plugin_get_info()"
        void *funcPointerGetInfo = dlsym(libPointer, "plugin_get_info");
        if (!funcPointerGetInfo)
        {
            fprintf(stderr, "ОШИБКА: не удалось найти функцию %s: %s\n", "plugin_get_info()", dlerror());
            goto CheckPluginEND;
        }
        typedef int (*functionGetInfo)(struct plugin_info *);
        functionGetInfo GetInfo = (functionGetInfo)funcPointerGetInfo;
        if (GetInfo(&pluginInfo) < 0)
        {
            fprintf(stderr, "ОШИБКА: Не удалось получить информацию о плагине!\n");
            goto CheckPluginEND;
        }
        fprintf(stdout, "Назначение плагина:\n%s\n", pluginInfo.plugin_purpose);
        fprintf(stdout, "Автор плагина:\n%s\n", pluginInfo.plugin_author);
        fprintf(stdout, "Поддерживаемые опции:\n");
        if (pluginInfo.sup_opts_len > 0)
            for (size_t i = 0; i < pluginInfo.sup_opts_len; i++)
                // TODO: проверить вывод опций
                fprintf(stdout, "\t--%s\t\t%s\n", pluginInfo.sup_opts[i].opt.name, pluginInfo.sup_opts[i].opt_descr);
        else
        {
            fprintf(stdout, "Нет поддерживаемых опций!\n");
            goto CheckPluginEND;
        }
#pragma endregion

#pragma region Проверка наличия и работоспособности функции "plugin_process_file()"
        //     // Get pointer to plugin_process_file()
        void *funcPointerProcessFile = dlsym(libPointer, "plugin_process_file");
        if (!funcPointerProcessFile)
        {
            fprintf(stderr, "ОШИБКА: не удалось найти функцию %s: %s\n", "plugin_process_file()", dlerror());
            goto CheckPluginEND;
        }

        typedef int (*functionProcessFile)(const char *, struct option *, size_t);
        functionProcessFile ProcessFile = (functionProcessFile)funcPointerProcessFile;

        // Prepare array of options for getopt_long
        longopts = calloc(pluginInfo.sup_opts_len + 1, sizeof(struct option));
        if (!longopts)
        {
            fprintf(stderr, "Ошбика: не удалось выполнить calloc(): %s\n", strerror(errno));
            goto CheckPluginEND;
        }

        // Copy option information
        for (size_t i = 0; i < pluginInfo.sup_opts_len; i++)
        {
            // Mind this!
            // getopt_long() requires array of struct option in its longopts arg,
            // but pluginInfo.sup_opts is array of plugin_option structs, not option structs.
            memcpy(longopts + i, &pluginInfo.sup_opts[i].opt, sizeof(struct option));
        }

        // Prepare array of actually used options that will be passed to
        // plugin_process_file() (Maximum pluginInfo.sup_opts_len options)
        opts_to_pass = calloc(pluginInfo.sup_opts_len, sizeof(struct option));
        if (!opts_to_pass)
        {
            fprintf(stderr, "Ошбика: не удалось выполнить calloc(): %s\n", strerror(errno));
            goto CheckPluginEND;
        }

        if (LAB1DEBUG == true)
            for (size_t i = 0; i < pluginInfo.sup_opts_len; i++)
            {
                fprintf(stderr, "DEBUG: to getopt(): passing option '%s'\n",
                        (longopts + i)->name);
            }

        // Now process options for the lib
        while (1)
        {
            int opt_ind = 0;
            ret = getopt_long(argc, argv, "", longopts, &opt_ind);
            if (ret == -1)
                break;

            if (ret != 0)
            {
                fprintf(stderr, "ERROR: failed to parse options\n");
                goto CheckPluginEND;
            }

            // #ifndef ALLOW_OPT_ABBREV
            //         // glibc quirk: no proper way to disable option abbreviations
            //         // https://stackoverflow.com/questions/5182041/turn-off-abbreviation-in-getopt-long-optarg-h
            //         int idx = (longopts + opt_ind)->has_arg ? 2 : 1;
            //         const char *actual_opt_name = argv[optind - idx] + 2; // +2 for -- before option
            //         const char *found_opt_name = (longopts + opt_ind)->name;
            //         if (strcmp(actual_opt_name, found_opt_name))
            //         {
            //             // It's probably abbreviated name, which we do not allow
            //             fprintf(stderr, "ERROR: unknown option: %s\n", argv[optind - idx]);
            //             goto CheckPluginEND;
            //         }
            // #endif

            //         // Check how many options we got up to this moment
            //         if ((size_t)opts_to_pass_len == pluginInfo.sup_opts_len)
            //         {
            //             fprintf(stderr, "ERROR: too many options!\n");
            //             goto CheckPluginEND;
            //         }

            //         // Add this option to array of options actually passed to plugin_process_file()
            //         memcpy(opts_to_pass + opts_to_pass_len, longopts + opt_ind, sizeof(struct option));
            //         // Argument (if any) is passed in flag
            //         if ((longopts + opt_ind)->has_arg)
            //         {
            //             // Mind this!
            //             // flag is of type int*, but we are passing char* here (it's ok to do so).
            //             (opts_to_pass + opts_to_pass_len)->flag = (int *)strdup(optarg);
            //         }
            //         opts_to_pass_len++;
            //     }

            //     if (getenv("LAB1DEBUG"))
            //     {
            //         fprintf(stderr, "DEBUG: opts_to_pass_len = %d\n", opts_to_pass_len);
            //         for (int i = 0; i < opts_to_pass_len; i++)
            //         {
            //             fprintf(stderr, "DEBUG: passing option '%s' with arg '%s'\n",
            //                     (opts_to_pass + i)->name,
            //                     (char *)(opts_to_pass + i)->flag);
            //         }
            //     }

            //     // Call plugin_process_file()
            //     errno = 0;
            //     ret = ppf_func(file_name, opts_to_pass, opts_to_pass_len);
            //     fprintf(stdout, "plugin_process_file() returned %d\n", ret);
            //     if (ret < 0)
            //     {
            //         fprintf(stdout, "Error information: %s\n", strerror(errno));
            //     }

#pragma endregion

#pragma region Завершение проверки и освобождение памяти
        CheckPluginEND:

            //     if (opts_to_pass)
            //     {
            //         for (int i = 0; i < opts_to_pass_len; i++)
            //             free((opts_to_pass + i)->flag);
            //         free(opts_to_pass);
            //     }
            // if (longopts)
            //     free(longopts);
            // if (libPath)
            //     free(libPath);
            // if (file_name)
            //     free(file_name);
            // if (libPointer)
            //     dlclose(libPointer);
            return false;
        }
#pragma endregion
        return false;
    }

#pragma endregion

#pragma endregion

    int main(int argc, char *argv[])
    {

#pragma region Проверка плагина
        CheckPlugin(argv, argc);
#pragma endregion

#pragma region Проверка и выбор опций
        // TODO: доделать проверку опций
        printf("TODO: доделать проверку опций!!!\n");
        // SelectOption(argv, argc);
        //  Minimum number of arguments is 4:
        //  $ program_name libPath --opt1 file_to_check
        //
        //  if (argc < 4)
        //  {
        //      fprintf(stdout, "Usage: lab1call /path/to/libabcNXXXX.so [options_for_lib] /path/to/file\n");
        //      return 0;
        //  }
#pragma endregion

        return 0;
    }

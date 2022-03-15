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
#include <alloca.h>
#include <dirent.h>
#include <dlfcn.h>
#include <errno.h>
#include <ftw.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int CallPlugin(char *argv[], int argc)
{
    int opts_to_pass_len = 0;
    struct option *opts_to_pass = NULL;
    struct option *longopts = NULL;

    // Minimum number of arguments is 4:
    // $ program_name lib_name --opt1 file_to_check
    if (argc < 4)
    {
        fprintf(stdout, "Usage: lab1call /path/to/libabcNXXXX.so [options_for_lib] /path/to/file\n");
        return 0;
    }

    // Name of the lib. Should be passed as the first argument.
    char *lib_name = strdup(argv[1]);

    // Name of the file to analyze. Should be passed as the last argumtent.
    char *file_name = strdup(argv[argc - 1]);

    struct plugin_info pi = {0};

    void *dl = dlopen(lib_name, RTLD_LAZY);
    if (!dl)
    {
        fprintf(stderr, "ERROR: dlopen() failed: %s\n", dlerror());
        goto END;
    }

    // Check for plugin_get_info() func
    void *func = dlsym(dl, "plugin_get_info");
    if (!func)
    {
        fprintf(stderr, "ERROR: dlsym() failed: %s\n", dlerror());
        goto END;
    }

    typedef int (*pgi_func_t)(struct plugin_info *);
    pgi_func_t pgi_func = (pgi_func_t)func;

    int ret = pgi_func(&pi);
    if (ret < 0)
    {
        fprintf(stderr, "ERROR: plugin_get_info() failed\n");
        goto END;
    }

    // Plugin info
    fprintf(stdout, "Plugin purpose:\t\t%s\n", pi.plugin_purpose);
    fprintf(stdout, "Plugin author:\t\t%s\n", pi.plugin_author);
    fprintf(stdout, "Supported options: ");
    if (pi.sup_opts_len > 0)
    {
        fprintf(stdout, "\n");
        for (size_t i = 0; i < pi.sup_opts_len; i++)
        {
            fprintf(stdout, "\t--%s\t\t%s\n", pi.sup_opts[i].opt.name, pi.sup_opts[i].opt_descr);
        }
    }
    else
    {
        fprintf(stdout, "none (!?)\n");
    }
    fprintf(stdout, "\n");

    // If library supports no options then we have to stop
    if (pi.sup_opts_len == 0)
    {
        fprintf(stderr, "ERROR: library supports no options! How so?\n");
        goto END;
    }

    // Get pointer to plugin_process_file()
    func = dlsym(dl, "plugin_process_file");
    if (!func)
    {
        fprintf(stderr, "ERROR: no plugin_process_file() function found\n");
        goto END;
    }

    typedef int (*ppf_func_t)(const char *, struct option *, size_t);
    ppf_func_t ppf_func = (ppf_func_t)func;

    // Prepare array of options for getopt_long
    longopts = calloc(pi.sup_opts_len + 1, sizeof(struct option));
    if (!longopts)
    {
        fprintf(stderr, "ERROR: calloc() failed: %s\n", strerror(errno));
        goto END;
    }

    // Copy option information
    for (size_t i = 0; i < pi.sup_opts_len; i++)
    {
        // Mind this!
        // getopt_long() requires array of struct option in its longopts arg,
        // but pi.sup_opts is array of plugin_option structs, not option structs.
        memcpy(longopts + i, &pi.sup_opts[i].opt, sizeof(struct option));
    }

    // Prepare array of actually used options that will be passed to
    // plugin_process_file() (Maximum pi.sup_opts_len options)
    opts_to_pass = calloc(pi.sup_opts_len, sizeof(struct option));
    if (!opts_to_pass)
    {
        fprintf(stderr, "ERROR: calloc() failed: %s\n", strerror(errno));
        goto END;
    }

    for (size_t i = 0; i < pi.sup_opts_len; i++)
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
            goto END;
        }

#ifndef ALLOW_OPT_ABBREV
        // glibc quirk: no proper way to disable option abbreviations
        // https://stackoverflow.com/questions/5182041/turn-off-abbreviation-in-getopt-long-optarg-h
        int idx = (longopts + opt_ind)->has_arg ? 2 : 1;
        const char *actual_opt_name = argv[optind - idx] + 2; // +2 for -- before option
        const char *found_opt_name = (longopts + opt_ind)->name;
        // printf("\n ACTUAL OPT NAME:'%s'\n", actual_opt_name);
        // printf("\n FOUND OPT NAME:'%s'\n", found_opt_name);
        if (strcmp(actual_opt_name, found_opt_name))
        {
            // It's probably abbreviated name, which we do not allow
            fprintf(stderr, "ERROR: unknown option: %s\n", argv[optind - idx]);
            goto END;
        }
#endif

        // Check how many options we got up to this moment
        if ((size_t)opts_to_pass_len == pi.sup_opts_len)
        {
            fprintf(stderr, "ERROR: too many options!\n");
            goto END;
        }

        // Add this option to array of options actually passed to plugin_process_file()
        memcpy(opts_to_pass + opts_to_pass_len, longopts + opt_ind, sizeof(struct option));
        // Argument (if any) is passed in flag
        if ((longopts + opt_ind)->has_arg)
        {
            // Mind this!
            // flag is of type int*, but we are passing char* here (it's ok to do so).
            (opts_to_pass + opts_to_pass_len)->flag = (int *)strdup(optarg);
        }
        opts_to_pass_len++;
    }

    if (getenv("LAB1DEBUG"))
    {
        fprintf(stderr, "DEBUG: opts_to_pass_len = %d\n", opts_to_pass_len);
        for (int i = 0; i < opts_to_pass_len; i++)
        {
            fprintf(stderr, "DEBUG: passing option '%s' with arg '%s'\n",
                    (opts_to_pass + i)->name,
                    (char *)(opts_to_pass + i)->flag);
        }
    }

    // Call plugin_process_file()
    errno = 0;
    ret = ppf_func(file_name, opts_to_pass, opts_to_pass_len);
    fprintf(stdout, "plugin_process_file() returned %d\n", ret);
    if (ret < 0)
    {
        fprintf(stdout, "Error information: %s\n", strerror(errno));
    }

END:
    if (opts_to_pass)
    {
        for (int i = 0; i < opts_to_pass_len; i++)
            free((opts_to_pass + i)->flag);
        free(opts_to_pass);
    }
    if (longopts)
        free(longopts);
    if (lib_name)
        free(lib_name);
    if (file_name)
        free(file_name);
    if (dl)
        dlclose(dl);

    return 0;
}

#pragma region Поиск плагинов
int ProcessFiles(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
{
    UNUSED(sb);
    UNUSED(ftwbuf);
    char *DEBUG = getenv("LAB1DEBUG");
    errno = 0;
    char indent[128] = {0};
    memset(indent, '\t', ftwbuf->level);       //табуляция в зависимости от "глубины" файла
    char start_esc[6] = {0}, end_esc[6] = {0}; //место для раскрашиивания

    if (typeflag == FTW_D || typeflag == FTW_DP || typeflag == FTW_DNR)
        return 0;

    int ret = 1;

    int *plugin_results = alloca(in_handles_len * sizeof(int));
    for (int i = 0; i < in_handles_len; ++i)
    {
        void *func = dlsym(in_handles[i], "plugin_process_file");
        if (!func)
        {
            fprintf(stderr, "ERROR: walk_func: no plugin_process_file() function found\n");
            return 0;
        }
        typedef int (*ppf_func_t)(const char *, struct option *, size_t);
        ppf_func_t ppf_func = (ppf_func_t)func;
        plugin_results[i] = ppf_func(fpath, plugin_in_opts[i], plugin_in_opts_len[i]);

        if (DEBUG)
            fprintf(stderr, "walk_func: plugin_process_file() returned %d\n", plugin_results[i]);
        if (errno == EINVAL || (errno == EINVAL && DEBUG))
        {
            fprintf(stderr, "ERROR: walk_func: file wasn't proccesed %s\n", strerror(errno));
            return 0;
        }
    }

    if (flagAO == 'A')
    {
        ret = 1;
        for (int i = 0; i < in_handles_len; ++i)
            ret = ret && !(plugin_results[i]);
    }
    else
    {
        ret = 0;
        for (int i = 0; i < in_handles_len; ++i)
            ret = ret || !(plugin_results[i]);
    }

    if (flagN)
        ret = !ret;

    if (ret)
    {
        sprintf(start_esc, "%s", "\033[35m"); //окрашивание в фиолетовый всех найденных файлов
        sprintf(end_esc, "%s", "\33[0m");
        fprintf(stdout, "%s%s%s%s\n", start_esc, indent, fpath, end_esc); //вывод полного пути к файлу
    }

    return 0;
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
    printf("Список аргументов:\n");
    for (int i = 0; i < argc; i++)
        printf("%s\n", argv[i]);
}
void SelectOption(char *argv[], int argc)
{
    if (LAB1DEBUG == true)
        PrintArgs(argv, argc);
    if (argc == 5)
    {
        CallPlugin(argv, argc);
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

int main(int argc, char *argv[])
{
    if (LAB1DEBUG == true)
        printf("_______Ввызов программы с несколькими плагинами______\n");
    SelectOption(argv, argc);
    return 0;
}

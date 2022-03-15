// https://habr.com/ru/post/321468/
//
//   main.c
//   FileFinder
//
//   Created by Карина Кондакова on 23.01.2022.
//
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
#define LAB1DEBUG false

#pragma region ИМЯ
#pragma endregion

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

#pragma region Поиск файлов с указанынм байтом
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
    fread(fileBytes, fileBytesCount, 1, file);
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
        perror("Can't open folder!");
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

#pragma region Распозначение опций
// -P dir Каталог с плагинами.
// -A Объединение опций плагинов с помощью операции «И» (действует по умолчанию).
// -O Объединение опций плагинов с помощью операции «ИЛИ».
// -N Инвертирование условия поиска(после объединения опций плагинов с помощью - A или - O).
// -v Вывод версии программы и информации о программе(ФИО исполнителя, номер группы, номер варианта лабораторной).
// -h Вывод справки по опциям
// --freq-byte <значение> <путь к каталогу с файлами>
void PrintArgs(const char *argv[], int argc)
{
    printf("Количество переданных аргументов:%i\n", argc);
    printf("Список аргументов:\n");
    for (int i = 0; i < argc; i++)
        printf("%s\n", argv[i]);
}
void SelectOption(const char *argv[], int argc)
{
    if (LAB1DEBUG == true)
        PrintArgs(argv, argc);
    if (argc == 4)
    {
        if (strcmp("--freq-byte", argv[1]) == 0)
        {
            bool isConverted = false;
            int byteValue = -1;
            if (IsStrDec(argv[2]))
            {
                isConverted = true;
                byteValue = DecStrToDec(argv[2]);
            }
            if (IsStrBin(argv[2]))
            {
                isConverted = true;
                byteValue = BinStrToDec(argv[2]);
            }
            if (IsStrHex(argv[2]))
            {
                isConverted = true;
                byteValue = HexStrToDec(argv[2]);
            }
            if (isConverted == false)
            {
                printf("Указан некорректный параметр для байта!");
                return;
            }
            struct StringList filesList = GetFiles(argv[3], byteValue);
            printf("Были найдены следующие файлы:\n");
            PrintStringList(filesList);
            DeleteStringList(&filesList);
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

#pragma endregion

#pragma region Тесты

#pragma region Вспомогательные методы
void PrintMessage(char *message)
{
    printf("%s\n", message);
}
char *Itoa(int value, int base)
{
    char *buffer = (char *)malloc(256 * sizeof(char));
    // invalid input
    if (base < 2 || base > 32)
        return buffer;
    // consider the absolute value of the number
    int n = abs(value);

    int i = 0;
    while (n)
    {
        int r = n % base;

        if (r >= 10)
        {
            buffer[i++] = 65 + (r - 10);
        }
        else
        {
            buffer[i++] = 48 + r;
        }
        n = n / base;
    }
    // if the number is 0
    if (i == 0)
    {
        buffer[i++] = '0';
    }
    // If the base is 10 and the value is negative, the resulting string
    // is preceded with a minus sign (-)
    // With any other base, value is always considered unsigned
    if (value < 0 && base == 10)
    {
        buffer[i++] = '-';
    }
    buffer[i] = '\0'; // null terminate string
    // reverse the string and return it
    for (int j = 0; j < i / 2; j++)
    {
        char temp = buffer[j];
        buffer[j] = buffer[i - j - 1];
        buffer[i - j - 1] = temp;
    }
    // return reverse(buffer, 0, i - 1);
    return buffer;
}
char *GenerateRandomStr(int length)
{
    char *alphabet = "1234567890qwertyuiopasdfghjklzxcvbnm";
    char *str = (char *)malloc((length + 1) * sizeof(char));
    str[length] = '\0';
    for (int i = 0; i < length; i++)
        str[i] = alphabet[rand() % strlen(alphabet)];
    return str;
}
#pragma endregion

void String_Test()
{
#pragma region Глобальные переменные
    bool isPassed = true;
    int testStringsCount = 1024;
    int testStringLength = 1024;
#pragma endregion

#pragma region Тело теста
    for (int i = 0; i < testStringsCount; i++)
    {
        char *testString = GenerateRandomStr(rand() % testStringLength);
        struct String str = InitString(testString);
        if (str.length != strlen(testString))
        {
            printf("Expected length:%lu, actual:%li\n", strlen(testString), str.length);
            isPassed = false;
            break;
        }
        if (strcmp(testString, str.items) != 0)
        {
            printf("Strings not equal!Expected%s, actual:%s\n", testString, str.items);
            isPassed = false;
            break;
        }
        DeleteString(&str);
        free(testString);
    }
#pragma endregion

#pragma region Вывод результата
    if (isPassed == true)
        PrintMessage("String_Test PASSED!");
    else
        PrintMessage("String_Test FAILED!");
#pragma endregion

#pragma region Очистка памяти

#pragma endregion
}
void StringList_Test()
{
#pragma region Глобальные переменные
    bool isPassed = true;
    int testStringsCount = 1024;
    int testStringLength = 1024;
    char **testStrings = (char **)malloc(testStringsCount * sizeof(char *));
    struct StringList stringList = InitStringList();
#pragma endregion

#pragma region Тело теста
    for (int i = 0; i < testStringsCount; i++)
        testStrings[i] = GenerateRandomStr(rand() % testStringLength);
    for (int i = 0; i < testStringsCount; i++)
        AddToStringList(&stringList, testStrings[i]);
    if (stringList.count != testStringsCount)
    {
        printf("Incorrect list length! Expected:%i, Actual:%i\n", testStringsCount, stringList.count);
        isPassed = false;
    }
    for (int i = 0; i < testStringsCount; i++)
    {
        char *testString = testStrings[i];
        struct String str = stringList.stirngs[i];
        if (str.length != strlen(testString))
        {
            printf("Expected length:%lu, actual:%li\n", strlen(testString), str.length);
            isPassed = false;
            break;
        }
        if (strcmp(testString, str.items) != 0)
        {
            printf("Strings not equal!Expected:%s, actual:%s\n", testString, str.items);
            isPassed = false;
            PrintStringList(stringList);
            break;
        }
    }
#pragma endregion

#pragma region Вывод результата
    if (isPassed == true)
        PrintMessage("StringList_Test PASSED!");
    else
        PrintMessage("StringList_Test FAILED!");
#pragma endregion

#pragma region Очистка памяти
    for (int i = 0; i < testStringsCount; i++)
        free(testStrings[i]);
    free(testStrings);
    DeleteStringList(&stringList);
#pragma endregion
}
void IsStrDec_Test()
{
#pragma region Глобальные переменные
    bool isPassed = true;
    int testIntCout = 1024;
    int testStringCount = 1024;
    int testStringLength = 256;
#pragma endregion

#pragma region Тело теста
    for (int i = 0; i < testIntCout; i++)
    {
        char *decStrValue = Itoa(i, 10);
        if (IsStrDec(decStrValue) != true)
        {
            printf("IsStrDec_Test expected TRUE for  %i value!\n", i);
            isPassed = false;
            break;
        }
        free(decStrValue);
    }
    for (int i = 0; i < testStringCount; i++)
    {
        char *randomStr = GenerateRandomStr(testStringLength);
        if (IsStrDec(randomStr) != false)
        {
            printf("IsStrDec_Test expected FALSE for  %s value!\n", randomStr);
            isPassed = false;
            break;
        }
        free(randomStr);
    }
#pragma endregion

#pragma region Вывод результата
    if (isPassed == true)
        PrintMessage("IsStrDec_Test PASSED!");
    else
        PrintMessage("IsStrDec_Test FAILED!");
#pragma endregion

#pragma region Очистка памяти

#pragma endregion
}
void IsStrBin_Test()
{
#pragma region Глобальные переменные
    bool isPassed = true;
    int testIntCout = 1024;
    int testStringCount = 1024;
    int testStringLength = 256;
#pragma endregion

#pragma region Тело теста
    for (int i = 0; i < testIntCout; i++)
    {
        char prefix[255] = "0b";
        char *binStrValue = Itoa(i, 2);
        strcat(prefix, binStrValue);
        if (IsStrBin(prefix) != true)
        {
            printf("IsStrBin_Test expected TRUE for  %i value!\n", i);
            isPassed = false;
            break;
        }
        free(binStrValue);
    }
    for (int i = 0; i < testStringCount; i++)
    {
        char *randomStr = GenerateRandomStr(testStringLength);
        if (IsStrBin(randomStr) != false)
        {
            printf("IsStrBin_Test expected FALSE for  %s value!\n", randomStr);
            isPassed = false;
            break;
        }
        free(randomStr);
    }

#pragma endregion

#pragma region Вывод результата
    if (isPassed == true)
        PrintMessage("IsStrBin_Test PASSED!");
    else
        PrintMessage("IsStrBin_Test FAILED!");
#pragma endregion

#pragma region Очистка памяти

#pragma endregion
}
void IsStrHex_Test()
{
#pragma region Глобальные переменные
    bool isPassed = true;
    int testIntCout = 1024;
    int testStringCount = 1024;
    int testStringLength = 256;
#pragma endregion

#pragma region Тело теста
    for (int i = 0; i < testIntCout; i++)
    {
        char prefix[255] = "0x";
        char *hexStrValue = Itoa(i, 16);
        strcat(prefix, hexStrValue);
        if (IsStrHex(prefix) != true)
        {
            printf("IsStrHex_Test expected TRUE for  %i value!\n", i);
            isPassed = false;
            break;
        }
        free(hexStrValue);
    }
    for (int i = 0; i < testStringCount; i++)
    {
        char *randomStr = GenerateRandomStr(testStringLength);
        if (IsStrHex(randomStr) != false)
        {
            printf("IsStrHex_Test expected FALSE for  %s value!\n", randomStr);
            isPassed = false;
            break;
        }
        free(randomStr);
    }
#pragma endregion

#pragma region Вывод результата
    if (isPassed == true)
        PrintMessage("IsStrHex_Test PASSED!");
    else
        PrintMessage("IsStrHex_Test FAILED!");
#pragma endregion

#pragma region Очистка памяти

#pragma endregion
}
void DecStrToDec_Test()
{
#pragma region Глобальные переменные
    bool isPassed = true;
    int testIntCout = 1024;
#pragma endregion

#pragma region Тело теста
    for (int i = 0; i < testIntCout; i++)
    {
        char *strValue = Itoa(i, 10);
        if (DecStrToDec(strValue) != i)
        {
            printf("DecStrToDec_Test expected TRUE for  %i value!\n", i);
            isPassed = false;
            break;
        }
        free(strValue);
    }
#pragma endregion

#pragma region Вывод результата
    if (isPassed == true)
        PrintMessage("DecStrToDec_Test PASSED!");
    else
        PrintMessage("DecStrToDec_Test FAILED!");
#pragma endregion

#pragma region Очистка памяти

#pragma endregion
}
void BinStrToDec_Test()
{
#pragma region Глобальные переменные
    bool isPassed = true;
    int testIntCout = 1024;
#pragma endregion

#pragma region Тело теста
    for (int i = 0; i < testIntCout; i++)
    {
        char prefix[255] = "0b";
        char *binStrValue = Itoa(i, 2);
        strcat(prefix, binStrValue);
        if (BinStrToDec(prefix) != i)
        {
            printf("BinStrToDec_Test expected TRUE for  %i value!\n", i);
            isPassed = false;
            break;
        }
        free(binStrValue);
    }

#pragma endregion

#pragma region Вывод результата
    if (isPassed == true)
        PrintMessage("BinStrToDec_Test PASSED!");
    else
        PrintMessage("BinStrToDec_Test FAILED!");
#pragma endregion

#pragma region Очистка памяти

#pragma endregion
}
void HexStrToDec_Test()
{
#pragma region Глобальные переменные
    bool isPassed = true;
    int testIntCout = 1024;
#pragma endregion

#pragma region Тело теста
    for (int i = 0; i < testIntCout; i++)
    {
        char prefix[255] = "0x";
        char *hexStrValue = Itoa(i, 16);
        strcat(prefix, hexStrValue);
        if (HexStrToDec(prefix) != i)
        {
            printf("HexStrToDec_Test expected TRUE for  %i value!\n", i);
            isPassed = false;
            break;
        }
        free(hexStrValue);
    }
#pragma endregion

#pragma region Вывод результата
    if (isPassed == true)
        PrintMessage("HexStrToDec_Test PASSED!");
    else
        PrintMessage("HexStrToDec_Test FAILED!");
#pragma endregion

#pragma region Очистка памяти

#pragma endregion
}
void GetMostByteFromFile_test()
{
#pragma region Глобальные переменные
    bool isPassed = true;
    int subTestResult = 0;
    char *testFilePath = "";
#pragma endregion

#pragma region Тело теста

#pragma region Тест на поиск байта со значением 49
    testFilePath = "./TestFiles/49/49.txt";
    subTestResult = GetMostByteFromFile(testFilePath);
    if (subTestResult != 49)
    {
        printf("Expected:%i Actual:%i in file:%s\n", 49, subTestResult, testFilePath);
        isPassed = false;
    }
#pragma endregion
#pragma region Тест на поиск байта со значением 50
    testFilePath = "./TestFiles/50/50.txt";
    subTestResult = GetMostByteFromFile(testFilePath);
    if (subTestResult != 50)
    {
        printf("Expected:%i Actual:%i in file:%s\n", 50, subTestResult, testFilePath);
        isPassed = false;
    }
#pragma endregion
#pragma region Тест на поиск байта со значением 51
    testFilePath = "./TestFiles/51/51.txt";
    subTestResult = GetMostByteFromFile(testFilePath);
    if (subTestResult != 51)
    {
        printf("Expected:%i Actual:%i in file:%s\n", 51, subTestResult, testFilePath);
        isPassed = false;
    }
#pragma endregion
#pragma region Тест на поиск байта со значением 52
    testFilePath = "./TestFiles/52/52.txt";
    subTestResult = GetMostByteFromFile(testFilePath);
    if (subTestResult != 52)
    {
        printf("Expected:%i Actual:%i in file:%s\n", 52, subTestResult, testFilePath);
        isPassed = false;
    }
#pragma endregion
#pragma region Тест на поиск байта со значением 53
    testFilePath = "./TestFiles/53/53.txt";
    subTestResult = GetMostByteFromFile(testFilePath);
    if (subTestResult != 53)
    {
        printf("Expected:%i Actual:%i in file:%s\n", 53, subTestResult, testFilePath);
        isPassed = false;
    }
#pragma endregion

#pragma endregion

#pragma region Вывод результата
    if (isPassed == true)
        PrintMessage("GetMostByteFromFile_test PASSED!");
    else
        PrintMessage("GetMostByteFromFile_test FAILED!");
#pragma endregion

#pragma region Очистка памяти

#pragma endregion
}
void GetFiles_Test()
{
#pragma region Глобальные переменные
    bool isPassed = true;
    struct StringList list1 = GetFiles("./TestFiles", 49);
    struct StringList list2 = GetFiles("./TestFiles", 50);
    struct StringList list3 = GetFiles("./TestFiles", 51);
    struct StringList list4 = GetFiles("./TestFiles", 52);
    struct StringList list5 = GetFiles("./TestFiles", 53);
#pragma endregion

#pragma region Тело теста
    if (list1.count != 6)
    {
        printf("In folder '49' Expected:%i , Actual:%i", 6, list1.count);
        isPassed = false;
    }
    if (list2.count != 3)
    {
        printf("In folder 50' Expected:%i , Actual:%i", 3, list2.count);
        isPassed = false;
    }
    if (list3.count != 4)
    {
        printf("In folder '51' Expected:%i , Actual:%i", 4, list3.count);
        isPassed = false;
    }
    if (list4.count != 2)
    {
        printf("In folder '52' Expected:%i , Actual:%i", 2, list4.count);
        isPassed = false;
    }
    if (list5.count != 8)
    {
        printf("In folder '53' Expected:%i , Actual:%i", 8, list5.count);
        isPassed = false;
    }
#pragma endregion

#pragma region Вывод результата
    if (isPassed == true)
        PrintMessage("GetFiles_Test PASSED!");
    else
        PrintMessage("GetFiles_Test FAILED!");
#pragma endregion

#pragma region Очистка памяти
    DeleteStringList(&list1);
    DeleteStringList(&list2);
    DeleteStringList(&list3);
    DeleteStringList(&list4);
    DeleteStringList(&list5);
#pragma endregion
}
void RunTests()
{
    srand(time(NULL));
    String_Test();
    StringList_Test();
    IsStrDec_Test();
    IsStrBin_Test();
    IsStrHex_Test();
    DecStrToDec_Test();
    BinStrToDec_Test();
    HexStrToDec_Test();
    GetMostByteFromFile_test();
    GetFiles_Test();
}

#pragma endregion
int main(int argc, const char *argv[])
{
    if (LAB1DEBUG == true)
        RunTests();
    SelectOption(argv, argc);
    return 0;
}

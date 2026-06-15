#include "stdafx.h"
#include "MatrixInt.h"
#include "String.h"
#include "StringList.h"
#include "StrHelper.h"
#include "Converter.h"
#include "FileIO.h"

void TestStringAppend(void)
{
	char* str2 = (char*)calloc(6, sizeof(char));
	strcpy(str2, "12345");
	str2 = StrAppendChar(str2, '9');
	printf("%s\n", str2);
	free(str2);

	int value = 12345;
	char* str = ConvertIntToStr(value);
	printf("%s\n", str);
	free(str);
}

void TestStringIsNumber1(void)
{
	StringList testStrings = StringListInit();

	StringListAdd(&testStrings, StringCopy("1234567890"));
	StringListAdd(&testStrings, StringCopy("+1234567890"));
	StringListAdd(&testStrings, StringCopy("-1234567890"));
	StringListAdd(&testStrings, StringCopy("+1234567890"));
	StringListAdd(&testStrings, StringCopy("1.123"));
	StringListAdd(&testStrings, StringCopy("1,123"));
	StringListAdd(&testStrings, StringCopy("+11.123"));
	StringListAdd(&testStrings, StringCopy("-12,123"));

	StringListAdd(&testStrings, StringCopy("qwerty"));
	StringListAdd(&testStrings, StringCopy("0987654321"));
	StringListAdd(&testStrings, StringCopy("-0987654321"));
	StringListAdd(&testStrings, StringCopy("-"));
	StringListAdd(&testStrings, StringCopy("+"));
	StringListAdd(&testStrings, StringCopy("+."));
	StringListAdd(&testStrings, StringCopy(".123.123"));
	StringListAdd(&testStrings, StringCopy("123.123.123"));
	StringListAdd(&testStrings, StringCopy("123+123.123"));

	bool expected[17] = {
		true,
		true,
		true,
		true,
		true,
		true,
		true,
		true,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false
	};

	int falseCount = 0;
	for (size_t i = 0; i < testStrings.length; i++)
	{
		if (StringIsNumber(&testStrings.items[i]) != expected[i])
		{
			String expectedValue = StringFromBoolean(expected[i]);
			printf("%s expected %s\n", testStrings.items[i].symbols, expectedValue.symbols);
			StringDelete(expectedValue);
			falseCount++;
		}
	}

	if (falseCount == 0)
	{
		printf("All string tests passed!\n");
	}

	StringListDelete(&testStrings);
}

void TestStringIsNumber2(void)
{
	String str = StringInit(0);
	char c;

	while ((c = getchar()) != '\n' && c != EOF)
	{
		StringAdd(&str, c);
	}

	String result = StringFromBoolean(StringIsNumber(&str));
	printf("%s", result.symbols);
	StringDelete(result);
	StringDelete(str);
}

void TestMatrix(void)
{
	MatrixInt matrix = MatrixIntInit(4, 5);
	MatrixIntInitRandom(&matrix, 0, 100);
	printf("Original:\n");
	MatrixIntPrint(&matrix);

	MatrixInt matrixTransposed = MatrixIntTransponse(&matrix);
	printf("Transposed:\n");
	MatrixIntPrint(&matrixTransposed);

	MatrixIntDelete(&matrix);
	MatrixIntDelete(&matrixTransposed);
}

void TestMatrixMultiply(void)
{
	MatrixInt a = MatrixIntInit(3, 4);
	MatrixIntInitRandom(&a, 0, 10);
	printf("A:\n");
	MatrixIntPrint(&a);

	MatrixInt b = MatrixIntInit(4, 3);
	MatrixIntInitRandom(&b, 0, 10);
	printf("B:\n");
	MatrixIntPrint(&b);

	MatrixInt c = MatrixIntMultiply(&a, &b);
	printf("C = A x B:\n");
	MatrixIntPrint(&c);

	MatrixIntDelete(&a);
	MatrixIntDelete(&b);
	MatrixIntDelete(&c);
}

int main(void)
{
	srand((unsigned int)time(NULL));
	setlocale(LC_ALL, "rus");

	TestStringIsNumber1();
	TestMatrixMultiply();

	return 0;
}

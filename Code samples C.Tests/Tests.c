#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Code samples C/Converter.h"
#include "../Code samples C/MatrixInt.h"
#include "../Code samples C/StrHelper.h"
#include "../Code samples C/String.h"
#include "../Code samples C/StringList.h"

static int failedAssertions = 0;

#define ASSERT_TRUE(condition) AssertTrue((condition), #condition, __FILE__, __LINE__)
#define ASSERT_INT(expected, actual) AssertInt((expected), (actual), #actual, __FILE__, __LINE__)
#define ASSERT_SIZE(expected, actual) AssertSize((expected), (actual), #actual, __FILE__, __LINE__)
#define ASSERT_STR(expected, actual) AssertString((expected), (actual), #actual, __FILE__, __LINE__)

static void AssertTrue(bool condition, const char* expression, const char* file, int line)
{
	if (!condition)
	{
		printf("%s:%d assertion failed: %s\n", file, line, expression);
		failedAssertions++;
	}
}

static void AssertInt(int expected, int actual, const char* expression, const char* file, int line)
{
	if (expected != actual)
	{
		printf("%s:%d assertion failed: %s expected %d actual %d\n", file, line, expression, expected, actual);
		failedAssertions++;
	}
}

static void AssertSize(size_t expected, size_t actual, const char* expression, const char* file, int line)
{
	if (expected != actual)
	{
		printf("%s:%d assertion failed: %s expected %zu actual %zu\n", file, line, expression, expected, actual);
		failedAssertions++;
	}
}

static void AssertString(const char* expected, const char* actual, const char* expression, const char* file, int line)
{
	if (actual == NULL || strcmp(expected, actual) != 0)
	{
		printf("%s:%d assertion failed: %s expected \"%s\" actual \"%s\"\n", file, line, expression, expected, actual == NULL ? "(null)" : actual);
		failedAssertions++;
	}
}

static void TestStringCopyAndAppend(void)
{
	String value = StringCopy("ab");
	StringAdd(&value, 'c');

	ASSERT_STR("abc", value.symbols);
	ASSERT_SIZE(3, value.length);
	ASSERT_SIZE(3, StringGetLength(value.symbols));

	StringDelete(value);
}

static void TestStringIsNumber(void)
{
	const char* valid[] = { "123", "+123", "-123", "1.5", "1,5" };
	const char* invalid[] = { "", "0123", "abc", "1.2.3", "12+3", "+.", "-" };

	for (size_t i = 0; i < sizeof(valid) / sizeof(valid[0]); i++)
	{
		String value = StringCopy(valid[i]);
		ASSERT_TRUE(StringIsNumber(&value));
		StringDelete(value);
	}

	for (size_t i = 0; i < sizeof(invalid) / sizeof(invalid[0]); i++)
	{
		String value = StringCopy(invalid[i]);
		ASSERT_TRUE(!StringIsNumber(&value));
		StringDelete(value);
	}
}

static void TestStringListOperations(void)
{
	StringList list = StringListInit();
	StringListAdd(&list, StringCopy("bb"));
	StringListAdd(&list, StringCopy("a"));
	StringListAdd(&list, StringCopy("ccc"));

	StringListSort(&list, true);

	ASSERT_SIZE(3, list.length);
	ASSERT_STR("a", list.items[0].symbols);
	ASSERT_STR("bb", list.items[1].symbols);
	ASSERT_STR("ccc", list.items[2].symbols);

	StringListDeleteAt(&list, 1);

	ASSERT_SIZE(2, list.length);
	ASSERT_STR("a", list.items[0].symbols);
	ASSERT_STR("ccc", list.items[1].symbols);

	StringListDelete(&list);
}

static void TestSplitStringToStringList(void)
{
	String source = StringCopy("one two  three");
	StringList words = SplitStringToStringList(source);

	ASSERT_SIZE(3, words.length);
	ASSERT_STR("one", words.items[0].symbols);
	ASSERT_STR("two", words.items[1].symbols);
	ASSERT_STR("three", words.items[2].symbols);

	StringListDelete(&words);
	StringDelete(source);
}

static void TestStringHelpersAndConverters(void)
{
	char* text = NULL;
	text = StrAppendChar(text, 'a');
	text = StrAppendStr(text, "bc");
	ASSERT_STR("abc", text);
	free(text);

	char* intValue = ConvertIntToStr(-42);
	char* floatValue = ConvertFloatToStr(1.5f);
	char* doubleValue = ConvertDoubleToStr(2.25);

	ASSERT_STR("-42", intValue);
	ASSERT_STR("1.5", floatValue);
	ASSERT_STR("2.25", doubleValue);

	free(intValue);
	free(floatValue);
	free(doubleValue);
}

static void TestMatrixTranspose(void)
{
	MatrixInt matrix = MatrixIntInit(2, 3);
	MatrixIntSet(&matrix, 0, 0, 1);
	MatrixIntSet(&matrix, 0, 1, 2);
	MatrixIntSet(&matrix, 0, 2, 3);
	MatrixIntSet(&matrix, 1, 0, 4);
	MatrixIntSet(&matrix, 1, 1, 5);
	MatrixIntSet(&matrix, 1, 2, 6);

	MatrixInt transposed = MatrixIntTransponse(&matrix);

	ASSERT_SIZE(3, transposed.rowsCount);
	ASSERT_SIZE(2, transposed.columnsCount);
	ASSERT_INT(1, *MatrixIntGet(&transposed, 0, 0));
	ASSERT_INT(4, *MatrixIntGet(&transposed, 0, 1));
	ASSERT_INT(3, *MatrixIntGet(&transposed, 2, 0));
	ASSERT_INT(6, *MatrixIntGet(&transposed, 2, 1));

	MatrixIntDelete(&transposed);
	MatrixIntDelete(&matrix);
}

static void TestMatrixMultiply(void)
{
	MatrixInt a = MatrixIntInit(2, 3);
	MatrixInt b = MatrixIntInit(3, 2);

	int aValues[2][3] = {
		{ 1, 2, 3 },
		{ 4, 5, 6 }
	};
	int bValues[3][2] = {
		{ 7, 8 },
		{ 9, 10 },
		{ 11, 12 }
	};

	for (size_t i = 0; i < 2; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			MatrixIntSet(&a, i, j, aValues[i][j]);
		}
	}
	for (size_t i = 0; i < 3; i++)
	{
		for (size_t j = 0; j < 2; j++)
		{
			MatrixIntSet(&b, i, j, bValues[i][j]);
		}
	}

	MatrixInt result = MatrixIntMultiply(a, b);

	ASSERT_SIZE(2, result.rowsCount);
	ASSERT_SIZE(2, result.columnsCount);
	ASSERT_INT(58, *MatrixIntGet(&result, 0, 0));
	ASSERT_INT(64, *MatrixIntGet(&result, 0, 1));
	ASSERT_INT(139, *MatrixIntGet(&result, 1, 0));
	ASSERT_INT(154, *MatrixIntGet(&result, 1, 1));

	MatrixIntDelete(&result);
	MatrixIntDelete(&b);
	MatrixIntDelete(&a);
}

int main(void)
{
	TestStringCopyAndAppend();
	TestStringIsNumber();
	TestStringListOperations();
	TestSplitStringToStringList();
	TestStringHelpersAndConverters();
	TestMatrixTranspose();
	TestMatrixMultiply();

	if (failedAssertions == 0)
	{
		printf("All unit tests passed.\n");
		return 0;
	}

	printf("%d unit test assertion(s) failed.\n", failedAssertions);
	return 1;
}

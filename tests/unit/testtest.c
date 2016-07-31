#include <string.h>
#include "CuTest.h"

char *StrToUpper(char **str) {
	return str;
}

void Test_StrToUpper(CuTest *tc) {
	char *input = strdup("Hello world");
	char *actual = StrToUpper(input);
	char *expected = "HELLO WORLD";

	CuAssertStrEquals(tc, expected, actual);
}

CuSuite *testtestGetSuite() {
	CuSuite *newSuite = CuSuiteNew();

	SUITE_ADD_TEST(newSuite, Test_StrToUpper);

	return newSuite;
}
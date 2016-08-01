#define _DEFAULT_SOURCE
#include <string.h>
#include <ctype.h>
#include "CuTest.h"
#include "testtest.h"
#include "hashmap.h"

char *StrToUpper(char **str) {

	for ( int i = 0; (*str)[i] != '\0'; ++i ) {
		(*str)[i] = toupper( (*str)[i] );
	}

	return *str;
}

void Test_StrToUpper(CuTest *tc) {
	char *input = strdup("Hello world");
	char *actual = StrToUpper(&input);
	char *expected = "HELLO WORLD";

	CuAssertStrEquals(tc, expected, actual);
}

CuSuite *testtestGetSuite() {
	CuSuite *newSuite = CuSuiteNew();

	SUITE_ADD_TEST(newSuite, Test_StrToUpper);

	return newSuite;
}
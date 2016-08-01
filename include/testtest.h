#ifndef TESTTEST_H
#define TESTTEST_H

#include "CuTest.h"

CuSuite *testtestGetSuite( void );
char *StrToUpper(char **);
void Test_StrToUpper(CuTest *);
CuSuite *testtestGetSuite(void);

#endif
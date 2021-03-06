#include <stdio.h>
#include "CuTest.h"
#include "testheader.h"

void RunAllTests( void );

void RunAllTests( void ) {
	CuString *output = CuStringNew();
	CuSuite *suite = CuSuiteNew();

	CuSuiteAddSuite(suite, getHashmapTestSuite() );

	CuSuiteRun( suite );
	CuSuiteSummary( suite, output );
	CuSuiteDetails( suite, output );

	puts( output->buffer );

	CuSuiteDelete( suite );
	CuStringDelete( output );
}

int main( void ) {
	RunAllTests();
	return 0;
}
#include "CuTest.h"
#include "hashmap.h"

#define HASHMAP_SIZE 256

void test_mapinit(CuTest *tc) {

	HM_HASHMAP * new_map = HM_initialize_hashmap(HASHMAP_SIZE);

	CuAssertPtrNotNull(tc, new_map);
	CuAssertPtrNotNull();

}

CuSuite *getHashmapTestSuite( void ) {
	CuSuite *newSuite = CuSuiteNew();

	SUITE_ADD_TEST(newSuite, test_mapinit);

	return newSuite;
}
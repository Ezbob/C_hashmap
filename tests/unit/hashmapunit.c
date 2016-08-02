#include "CuTest.h"
#include "hashmap.h"
#include "testheader.h"

#define HASHMAP_SIZE 256
void test_mapinit(CuTest *tc);


void test_mapinit(CuTest *tc) {

	HM_HASHMAP * new_map = HM_initialize_hashmap(HASHMAP_SIZE);

	CuAssertPtrNotNull(tc, new_map);
	CuAssertTrue(tc, new_map->entries_used == 0);
	CuAssertTrue(tc, new_map->hash_size == HASHMAP_SIZE);

	for ( size_t i = 0; i < new_map->hash_size; ++i ) {
		CuAssertTrue( tc, new_map->entries[i] == NULL );
	}

	HM_destroyHashmap( &new_map );
}

CuSuite *getHashmapTestSuite( void ) {
	CuSuite *newSuite = CuSuiteNew();

	SUITE_ADD_TEST(newSuite, test_mapinit);

	return newSuite;
}
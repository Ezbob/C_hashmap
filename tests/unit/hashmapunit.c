#define _DEFAULT_SOURCE
#include <string.h>
#include "CuTest.h"
#include "hashmap.h"
#include "testheader.h"

#define HASHMAP_SIZE 256
#define STRING_SIZE 64

void test_mapInit(CuTest *tc);
void test_mapAdd(CuTest *tc);
void test_mapAddCollision(CuTest *tc);
void test_mapAddGet(CuTest *tc);
void test_mapPollCollision(CuTest *tc);
void test_mapDropBucket(CuTest *tc);
void test_copyHashMap(CuTest *tc);

void test_mapInit(CuTest *tc) {

	HM_HASHMAP *new_map = HM_initialize_hashmap(HASHMAP_SIZE);

	CuAssertPtrNotNull(tc, new_map);
	CuAssertTrue(tc, new_map->entries_used == 0);
	CuAssertTrue(tc, new_map->hash_size == HASHMAP_SIZE);

	for ( size_t i = 0; i < new_map->hash_size; ++i ) {
		CuAssertTrue( tc, new_map->entries[i] == NULL );
	}

	HM_destroyHashmap( &new_map );
}

void test_mapAdd(CuTest *tc) {

	HM_HASHMAP *new_map = HM_initialize_hashmap(HASHMAP_SIZE);

	char *key = strdup("Hello world");
	int value = 42;

	HM_putValue(new_map, key, ( (void *) &value ) );

	CuAssertPtrNotNull(tc, new_map);
	CuAssertTrue(tc, new_map->entries_used == 1);


	HM_destroyHashmap( &new_map );
	free(key);
}

void test_mapAddCollision(CuTest *tc) {

	HM_HASHMAP *new_map = HM_initialize_hashmap(HASHMAP_SIZE);

	char *key = strdup("Hello world");
	int value = 42;

	char *key2 = strdup("Hello world");
	int value2 = 37;

	HM_putValue(new_map, key, ( (void *) &value ) );
	HM_putValue(new_map, key2, ( (void *) &value2 ) );

	CuAssertPtrNotNull(tc, new_map);
	CuAssertTrue(tc, new_map->entries_used == 1);
	CuAssertTrue(tc, HM_getChainLength(new_map, "Hello world") == 2);
	
	HM_destroyHashmap( &new_map );

	free(key);
	free(key2);
}

void test_mapAddGet(CuTest *tc) {

	HM_HASHMAP *new_map = HM_initialize_hashmap(HASHMAP_SIZE);

	char *key = strdup("Hello world");
	int value = 42;

	HM_putValue(new_map, key, ( (void *) &value ) );

	CuAssertPtrNotNull(tc, new_map);
	CuAssertTrue(tc, new_map->entries_used == 1);

	HM_VALUE *result = HM_getValue(new_map, key);

	CuAssertPtrNotNull(tc, result->value);
	CuAssertTrue(tc, result->next == NULL);
	CuAssertTrue(tc, *( (int *) result->value ) == 42);

	HM_destroyHashmap( &new_map );
	free(key);
}

void test_mapPollCollision(CuTest *tc) {

	HM_HASHMAP *new_map = HM_initialize_hashmap(HASHMAP_SIZE);

	char *key = strdup("Hello world");
	int values[] = { 42, 37, 65 };

	HM_putValue(new_map, key, ( (void *) &values[0] ) );
	HM_putValue(new_map, key, ( (void *) &values[1] ) );
	HM_putValue(new_map, key, ( (void *) &values[2] ) );

	CuAssertPtrNotNull(tc, new_map);
	CuAssertTrue(tc, new_map->entries_used == 1);
	CuAssertTrue(tc, HM_getChainLength(new_map, "Hello world") == 3);

	CuAssertTrue(tc, *( ( int *) HM_poll(new_map, key) ) == values[0] );
	CuAssertTrue(tc, *( ( int *) HM_poll(new_map, key) ) == values[1] );
	CuAssertTrue(tc, *( ( int *) HM_poll(new_map, key) ) == values[2] );

	CuAssertTrue(tc, new_map->entries_used == 0);
	
	HM_destroyHashmap( &new_map );
	free(key);
}

void test_mapDropBucket(CuTest *tc) {

	HM_HASHMAP *new_map = HM_initialize_hashmap(HASHMAP_SIZE);

	char *key = strdup("Hello world");
	int values[] = { 42, 37, 65 };

	HM_putValue(new_map, key, ( (void *) &values[0] ) );
	HM_putValue(new_map, key, ( (void *) &values[1] ) );
	HM_putValue(new_map, key, ( (void *) &values[2] ) );

	CuAssertPtrNotNull(tc, new_map);
	CuAssertTrue(tc, new_map->entries_used == 1);
	CuAssertTrue(tc, HM_getChainLength(new_map, "Hello world") == 3);

	int wasDropped = HM_dropBucket(new_map, "Hello world");

	CuAssertTrue(tc, wasDropped);
	CuAssertTrue(tc, new_map->entries_used == 0);
	CuAssertTrue(tc, HM_getValue(new_map, "Hello world") == NULL);
	HM_destroyHashmap( &new_map );
	free(key);
}

void test_copyHashMap(CuTest *tc) {

	HM_HASHMAP *new_map = HM_initialize_hashmap(HASHMAP_SIZE);

	char *key = strdup("Hello world");
	int values[] = { 42, 37, 65, 1 };

	HM_putValue(new_map, key, ( (void *) &values[0] ) );
	HM_putValue(new_map, key, ( (void *) &values[1] ) );
	HM_putValue(new_map, key, ( (void *) &values[2] ) );
	HM_putValue(new_map, key, ( (void *) &values[3] ) );

	CuAssertPtrNotNull(tc, new_map);
	CuAssertTrue(tc, new_map->entries_used == 1);
	CuAssertTrue(tc, HM_getChainLength(new_map, "Hello world") == 4);

        HM_HASHMAP *copy_map = HM_copyHashmap(new_map);

	CuAssertPtrNotNull(tc, copy_map);
	CuAssertTrue(tc, copy_map->entries_used == 1);
	CuAssertTrue(tc, HM_getChainLength(copy_map, "Hello world") == 4);

	CuAssertTrue(tc, *( ( int *) HM_poll(copy_map, key) ) == values[0] );
	CuAssertTrue(tc, *( ( int *) HM_poll(copy_map, key) ) == values[1] );
	CuAssertTrue(tc, *( ( int *) HM_poll(copy_map, key) ) == values[2] );
	CuAssertTrue(tc, *( ( int *) HM_poll(copy_map, key) ) == values[3] );

	CuAssertTrue(tc, copy_map->entries_used == 0);

	HM_destroyHashmap(&copy_map);
	HM_destroyHashmap(&new_map);
        free(key);
}

CuSuite *getHashmapTestSuite( void ) {
	CuSuite *newSuite = CuSuiteNew();

	SUITE_ADD_TEST(newSuite, test_mapInit);
	SUITE_ADD_TEST(newSuite, test_mapAdd);
	SUITE_ADD_TEST(newSuite, test_mapAddCollision);
	SUITE_ADD_TEST(newSuite, test_mapAddGet);
	SUITE_ADD_TEST(newSuite, test_mapPollCollision);
	SUITE_ADD_TEST(newSuite, test_mapDropBucket);
        SUITE_ADD_TEST(newSuite, test_copyHashMap);

	return newSuite;
}

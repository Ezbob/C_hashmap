/**
 * A chained hash map 
 * Plan: the HM_VALUE struct forms the linked list structure while the HM_ENTRY act as
 * control element for the list (meaning it holds meta info).
 * User gets HM_VALUE back as a collection when quering for a entry. 
 */
#ifndef HASHMAP_H
#define HASHMAP_H
#include <stdlib.h>

typedef struct HM_VALUE {
	void *value;
	struct HM_VALUE *next;
} HM_VALUE;

typedef struct HM_ENTRY {
	char *key;

	size_t chain_length;
	struct HM_VALUE *value;
} HM_ENTRY;

typedef struct HM_HASHMAP {
	HM_ENTRY **entries;
	size_t entries_used;
	size_t hash_size;
} HM_HASHMAP;

/*
 * get the hash table size
 */
size_t HM_getHashSize(HM_HASHMAP *);

/*
 * get the hash table size
 */
size_t HM_getChainLength(HM_HASHMAP *, char *key);

/*
 * allocates heap memory for a new hashmap
 */
HM_HASHMAP *HM_initialize_hashmap(size_t hash_size);

/*
 * look up in hash table
 */
HM_VALUE *HM_getValue(HM_HASHMAP *, char *key);

/*
 * add to hash table
 */
void HM_putValue(HM_HASHMAP *, char *key, void *value);

/*
 * Get and remove the first value in the specific bucket 
 */
void *HM_poll(HM_HASHMAP *, char *key);

/*
 * Removes a specific value from hash table.
 */
int HM_removeValue(HM_HASHMAP *, char *key, void *value);

/*
 * Empties the whole bucket. The values contained  
 */
int HM_emptyBucket(HM_HASHMAP *, char *key);

/*
 * remove key-value pair from hash table and free it using the supplied destructor
 */
int HM_destroyValue(HM_HASHMAP *, char *key, void *value, void *(*destructer)( void *value ) );

/*
 * remove allocated heap memory for hashmap
 */
void HM_destroyHashmap(HM_HASHMAP *);

#endif

#ifndef HASHMAP_H
#define HASHMAP_H
#include <stdlib.h>

typedef struct HM_ENTRY {
	char *key;
	void *value;

	size_t chain_size;
	struct HM_ENTRY *next;
} HM_ENTRY;

typedef struct HM_HASHMAP {
	HM_ENTRY **entries;
	size_t hash_size;
} HM_HASHMAP;

/*
 * the hash function 
 */
unsigned int HM_hash(HM_HASHMAP *, char *key);

/*
 * get the hash table size
 */
size_t HM_getHashSize(HM_HASHMAP *);

/*
 * for a spicific hash entry, get the chain size
 */
size_t HM_getChainSize(HM_HASHMAP *, char *key);

/*
 * allocates heap memory for a new hashmap
 */
HM_HASHMAP *HM_initialize_hashmap(size_t hash_size);

/*
 * look up in hash table
 */
void *HM_getValue(HM_HASHMAP *, char *key);

/*
 * add to hash table
 */
void HM_putValue(HM_HASHMAP *, void *value);

/*
 * remove from hash table
 */
void HM_removeValue(HM_HASHMAP *, char *key);

/*
 * remove allocated heap memory for hashmap
 */
void HM_destroy_hashmap(HM_HASHMAP *);

#endif

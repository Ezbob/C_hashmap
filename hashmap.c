#include "hashmap.h"
#include <stdlib.h>
#include <string.h>

unsigned int HM_hash(HM_HASHMAP *map, char *key) {
	unsigned int i;
	unsigned int result = 0;
	size_t str_size = strlen(key);

	for (i = 0 ; i < str_size; ++i) {
		result += key[i];
		if(i < str_size-1){
			result <<= 1; // does not shift the last iteration
		}
	}
	return result % map->hash_size;
}

HM_HASHMAP *HM_initialize_hashmap(size_t hash_size) {
	HM_HASHMAP *new_map;
	int i;

	new_map = malloc(sizeof(HM_HASHMAP));	
	new_map->hash_size = hash_size;

	for (i = 0; i < hash_size; ++i){
		new_map->entries[i] = NULL;
	}

	return new_map;
}

size_t HM_getHashSize(HM_HASHMAP *map) {
	return  map->hash_size;
}

size_t HM_getChainSize(HM_HASHMAP *map, char *key) {
	return map->entries[HM_hash(map,key)]->chain_size;
}

void *HM_getValue(HM_HASHMAP *map, char *key) {
	HM_ENTRY *entry = map->entries[HM_hash(map,key)]; 
	for (int i = 0; i < entry->chain_size; ++i) {
		if ( entry->key == key ) {
			return entry->value;
		}
		entry = entry->next;
	}
	return NULL;
}

void HM_putValue(HM_HASHMAP *map, void *value) {

}

void HM_removeValue(HM_HASHMAP *map, char *key) {

}

void HM_destroy_hashmap(HM_HASHMAP *map) {

}

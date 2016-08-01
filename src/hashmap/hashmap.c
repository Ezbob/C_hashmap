#include "hashmap.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static HM_VALUE *HM_newValue( void *value, HM_VALUE *next );
static void HM_deleteValue( HM_VALUE *value);
static unsigned int HM_hash( size_t hash_size, char *key );
static HM_ENTRY *HM_newEntry( char *key, HM_VALUE *value );
static void HM_deleteEntry( HM_ENTRY *entry );

#define HM_ERROR( msg ) fprintf( stderr, msg "\n" )
#define HM_LOOKUP( map, key ) ( map->entries[ HM_hash( HM_getHashSize( map ), key ) ] )

static HM_VALUE *HM_newValue( void *value, HM_VALUE *next ) {
	HM_VALUE *new_val = malloc( sizeof(HM_VALUE) );
	if ( new_val == NULL ) {
		HM_ERROR( "Memory error: Cannot allocate new value" );
		return NULL;
	}

	new_val->value = value;
	new_val->next = next;

	return new_val;
}

static void HM_deleteValue( HM_VALUE *value ) {
	value->value = NULL;
	value->next = NULL;
	free(value);
}

static HM_ENTRY *HM_newEntry( char *key, HM_VALUE *value ) {
	HM_ENTRY *new_entry = malloc( sizeof( HM_ENTRY ) );
	if ( new_entry == NULL ) {
		HM_ERROR( "Memory error: Cannot allocate new entry" );
		return NULL;
	}
	new_entry->key = key;
	new_entry->value = value;

	if ( value == NULL ) {
		new_entry->chain_length = 0;	
	} else {
		new_entry->chain_length = 1;
	}

	return new_entry;
}

static void HM_deleteEntry( HM_ENTRY *entry ) {

	entry->key = NULL;
	if ( entry->value != NULL ) {
		HM_deleteValue( entry->value );
	} 
	free(entry);
}

static unsigned int HM_hash( size_t hash_size, char *key ) {
	size_t i;
	unsigned int result = 0;
	size_t str_size = strlen( key );

	for ( i = 0; i < str_size; ++i ) {
		result += key[i];
		if ( i < str_size - 1 ) {
			result <<= 1; // does not shift the last iteration
		}
	}
	return result % hash_size;
}

HM_HASHMAP *HM_initialize_hashmap( size_t hash_size ) {
	HM_HASHMAP *new_map;
	size_t i;

	new_map = malloc( sizeof( HM_HASHMAP ) );	
	new_map->hash_size = hash_size;
	new_map->entries_used = 0;

	for ( i = 0; i < hash_size; ++i ) {
		new_map->entries[i] = NULL;
	}

	return new_map;
}

size_t HM_getChainLength( HM_HASHMAP *map, char *key ) {
	return HM_LOOKUP(map, key)->chain_length;
}

size_t HM_getHashSize( HM_HASHMAP *map ) {
	return map->hash_size;
}

HM_VALUE *HM_getValue( HM_HASHMAP *map, char *key ) {
	HM_ENTRY *entry = HM_LOOKUP( map, key ); 
	return entry->value;
}

void HM_putValue( HM_HASHMAP *map, char *key, void *value ) {
	HM_ENTRY *entry = HM_LOOKUP( map, key );
	
	if ( entry == NULL ) {
		// new entry filled
		HM_ENTRY *new_entry = HM_newEntry( key, HM_newValue( value, NULL ) );
		HM_LOOKUP( map, key ) = new_entry;

		map->entries_used++;
	} else {
		HM_VALUE *iterator = entry->value;
		while ( iterator->next != NULL ) {
			iterator = iterator->next;
		}
		iterator->next = HM_newValue( value, NULL );

		entry->chain_length++;
	}
}

void *HM_poll( HM_HASHMAP *map, char *key ) {
	HM_ENTRY *entry = HM_LOOKUP( map, key );
	void *result = NULL;

	if ( entry == NULL ) {
		return result;
	}
	
	if ( entry->chain_length > 0 ) {
		HM_VALUE *value = entry->value;
		HM_VALUE *next = value->next;
		result = value->value;

		HM_deleteValue( value );
		
		entry->value = next;
		entry->chain_length--;

		if ( entry->chain_length == 0 ) {
			HM_deleteEntry(entry);
			HM_LOOKUP(map, key) = NULL;
			map->entries_used--;
		}
	}

	return result;
}

int HM_destroyValue(HM_HASHMAP *map, char *key, void *value, void *(*destructer)( void *value ) ) {
	return 0;
}

void HM_destroyHashmap( HM_HASHMAP *map ) {
	
}

#include "collections.h"

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <inttypes.h>
#include <stdio.h>

void* checked_malloc(size_t size) {
    void* ptr = malloc(size);
    if (ptr == NULL) {
        exit(errno);
    }

    return ptr;
}

static uint64_t hash(struct hashable* self) {
    if (self->precomputed_hash > 0) {
        return self->precomputed_hash;
    }

    uint64_t hash = 0xcbf29ce484222325;
    uint64_t FNV_prime = 0x100000001b3;

    for (int i = 0; i < self->len; i++) {
        hash = hash ^ ((uint8_t*)self->data)[i];
        hash = hash * FNV_prime;
    }

    self->precomputed_hash = hash;

    return hash;
}

static void destroy_hashable(struct hashable** self) {
    free((*self)->data);
    free(*self);
}

/*
 * Create a hashable
 */
hashable_t *hashable_init(void* data, size_t len) {
    hashable_t* hashable = checked_malloc(sizeof(hashable_t));
    hashable->data = checked_malloc(len);
    memcpy(hashable->data, data, len);

    hashable->hash = hash;
    hashable->len = len;
    hashable->destroy = destroy_hashable;
    hashable->precomputed_hash = 0;

    return hashable;
}

typedef struct {
    hashable_t* key;
    void* value;
    size_t value_size;
} kvp_t;

static void destroy_kvp(void* k) {
    kvp_t* kvp = (kvp_t*)k;
    kvp->key->destroy(&(kvp->key));
    free(kvp->value);
}

typedef struct {
    collection_t* kvp_list;
} bucket_t;

typedef struct hash_map{
    collection_t* buckets;
    size_t len;
    size_t keys_len;
    size_t value_size;
} private_hash_map_t;

static collection_t* init_buckets(size_t len) {
    collection_t* buckets = new_array_list(sizeof(bucket_t), NULL);

    for(int i = 0; i < len; i++) {
        bucket_t bucket;
        bucket.kvp_list = new_array_list(sizeof(kvp_t), destroy_kvp);
        buckets->add(buckets, &bucket);
    }

    return buckets;
}

/*
 * Initialize a new hashmap.
 */
hash_map_t* hash_map_init(size_t value_size) {
    private_hash_map_t *hash_map = checked_malloc(sizeof(private_hash_map_t));
    hash_map->buckets = init_buckets(16);

    hash_map->len = 16;
    hash_map->keys_len = 0;
    hash_map->value_size = value_size;

    return (hash_map_t)(hash_map);
}

/*
 * Retrieve a value from the given key, or NULL if the key does not exist.
 */
void* hash_map_get(hash_map_t* self, hashable_t *key) {
    private_hash_map_t* hash_map = (private_hash_map_t*)self;
    size_t index = key->hash(key) % hash_map->buckets->count(hash_map->buckets);
    bucket_t* bucket = (bucket_t*)(hash_map->buckets->at(hash_map->buckets, index));

    for(size_t i = 0; i < bucket->kvp_list->count(bucket->kvp_list); i++) {
        kvp_t* kvp = bucket->kvp_list->at(bucket->kvp_list, i);

        if((kvp->key->len == key->len) && (memcmp(kvp->key->data, key->data, key->len) == 0)) {
            return kvp->value;
        }
    }

    return NULL;
}

/* Returns true if a record is added */
static bool buckets_add_kvp(collection_t* buckets, kvp_t* kvp) {
    size_t count = buckets->count(buckets);
    size_t index = kvp->key->hash(kvp->key) % count;
    bucket_t* bucket = buckets->at(buckets, index);
    collection_t* kvp_list = bucket->kvp_list;

    if(kvp_list->count(kvp_list) == 0) {
        kvp_list->add(kvp_list, kvp);
        return true;
    }

    for(int i = 0; i < kvp_list->count(kvp_list); i++) {
        kvp_t* old_kvp = (kvp_t*)(kvp_list->at(kvp_list, i));

        if(old_kvp->key->len != kvp->key->len) {
            continue;
        }

        if(memcmp(old_kvp->key->data, kvp->key->data, kvp->key->len) == 0) {
            kvp_list->set(kvp_list, kvp, i);
            return false;
        }
    }

    kvp_list->add(kvp_list, kvp);
    return true;
}

static kvp_t* init_kvp(hashable_t *key, void* value, size_t value_size) {
    kvp_t* new_kvp = checked_malloc(sizeof(kvp_t));
    new_kvp->key = hashable_init(key->data, key->len);
    new_kvp->value_size = value_size;
    new_kvp->value = checked_malloc(value_size);
    memcpy(new_kvp->value, value, value_size);
    return new_kvp;
}

collection_t* rebuild_buckets(collection_t* old, size_t new_size) {
    collection_t* buckets = init_buckets(new_size);
    for (size_t i = 0; i < buckets->count(old); i++)
    {
        collection_t* kvps = ((bucket_t*)(old->at(old, i)))->kvp_list;
        for (size_t j = 0; j < kvps->count(kvps); j++) {
            kvp_t* old_kvp = kvps->at(kvps, j);
            kvp_t* new_kvp = init_kvp(old_kvp->key, old_kvp->value, old_kvp->value_size);
            buckets_add_kvp(buckets, new_kvp);
        }
    }

    return buckets;
}

/*
 * Set the value for a given key.
 * This will change the value if the key already exists.
 */
void hash_map_set(hash_map_t* self, hashable_t* key, void* value) {
    private_hash_map_t* priv = (private_hash_map_t*)self;
    size_t count =  priv->buckets->count(priv->buckets);

    if ((float)(priv->keys_len) / (float)(count) >= .75) {
        collection_t* buckets = rebuild_buckets(priv->buckets, count * 2);
        priv->buckets->cleanup(&(priv->buckets));
        priv->buckets = buckets;
    }

    kvp_t* new_kvp = init_kvp(key, value, priv->value_size);

    bool key_added = buckets_add_kvp(priv->buckets, new_kvp);
    if (key_added) {
        priv->keys_len++;
    }

    return;
}

/*
 * Retrieve the collection of hashable_t items used as keys.
 */
collection_t* hash_map_keys(hash_map_t* self) {

    return NULL; /*stub*/
}

/*
 * Free the hash map.
 */
void hash_map_destroy(hash_map_t* self) {

    return; /*stub*/
}

#include "collections.h"

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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

static void destroy(struct hashable* self) {
    free(self->data);
    free(self);
}

/*
 * Create a hashable
 */
hashable_t *hashable_init(uint8_t* data, size_t len) {
    hashable_t* hashable = malloc(sizeof(hashable_t));
    hashable->data = malloc(len);
    memcpy(hashable->data, data, len);

    hashable->hash = hash;
    hashable->destroy = destroy;
    hashable->precomputed_hash = 0;

    return hashable;
}

typedef struct {
    hashable_t* key;
    uint8_t* value;
    size_t value_size;
}kvp_t;

static void destroy_kvp(kvp_t* kvp) {
    kvp->key->destroy(kvp->key);
    free(kvp->value);
    free(kvp);
}

typedef struct {
    collection_t* kvp_list;
}bucket_t;

typedef struct hash_map{
    collection_t* buckets;
    size_t len;
    size_t keys_len;
    size_t value_size;
} private_hash_map_t;

static collection_t* init_buckets(size_t len) {
    collection_t* buckets =  new_array_list(sizeof(bucket_t));

    for(int i = 0; i < len; i++) {
        bucket_t bucket;
        bucket.kvp_list = new_array_list(sizeof(kvp_t));
        buckets->add(buckets, &bucket);
    }

    return buckets;
}

/*
 * Initialize a new hashmap.
 */
hash_map_t* hash_map_init(size_t value_size) {
    private_hash_map_t *hash_map = malloc(sizeof(private_hash_map_t));
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
    uint64_t hash = key->hash(key) % hash_map->len;
    bucket_t* bucket = (bucket_t*)(hash_map->buckets->at(hash_map->buckets, hash));


    for(size_t i = 0; i < bucket->kvp_list->count(bucket->kvp_list); i++) {
        kvp_t* kvp = bucket->kvp_list->at(bucket->kvp_list, i);

        if((kvp->key->len == key->len) && (memcmp(kvp->key->data, key->data, key->len) == 0)) {
            return kvp->value;
        }
    }

    return NULL;
}

static void buckets_add_kvp(collection_t* buckets, kvp_t* kvp) {
    size_t count = buckets->count(buckets);
    uint64_t index = kvp->key->hash(kvp->key) % count;
    collection_t* kvp_list = buckets->at(buckets, index);

    if(kvp_list->count == 0) {
        kvp_list->add(kvp_list, kvp);

        return;
    }

    for(int i = 0; i < kvp_list->count(kvp_list); i++) {
        kvp_t* old_kvp = (kvp_t*)(kvp_list->at(kvp_list, i));

        if(old_kvp->key->len != kvp->key->len) {
            continue;
        }

        if(memcmp(old_kvp->key, kvp->key, kvp->key->len) == 0) {
            destroy_kvp(old_kvp);
            kvp_list->set(kvp_list, kvp, sizeof(kvp_t));
            return;
        }
    }
    kvp_list->add(kvp_list, kvp);
}
/*
 * Set the value for a given key.
 * This will change the value if the key already exists.
 */
void hash_map_set(hash_map_t* self, hashable_t* key, void* value) {
    private_hash_map_t* priv = (private_hash_map_t*)self;
    size_t count =  priv->buckets->count(priv->buckets);

    if ((float)(priv->keys_len) / (float)(count) >= .75) {
        collection_t* buckets = init_buckets(count * 2);
        for(int i = 0; i < count; i++) {
            collection_t* kvp_list = ((bucket_t*)(priv->buckets->at(priv->buckets, i)))->kvp_list;
            for(int j = 0; j < kvp_list->count(kvp_list); j++) {
                kvp_t* kvp = kvp_list->at(kvp_list, j);
                buckets_add_kvp(buckets, kvp);
            }
        }
        priv->buckets->cleanup(priv->buckets, NULL);
        priv->buckets = buckets;
    } 

    kvp_t* new_kvp = malloc(sizeof(kvp_t*));
    new_kvp->key = key;
    new_kvp->value = value;
    new_kvp->value_size = priv->value_size;

    buckets_add_kvp(priv->buckets, new_kvp);

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
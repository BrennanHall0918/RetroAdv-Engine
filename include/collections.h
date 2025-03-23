#ifndef _COLLECTIONS_H_
#define _COLLECTIONS_H_

#include <stdlib.h>
#include <stdbool.h>

/*
 * A generic collection interface.
 */
typedef struct collection {
    /*
     * Append an item to the end of the collection.
     */
    void (*add)(struct collection* self, void* item);

    /*
     * Execute the provided function for each element in the collection,
     * return true if any_func returns true for any element.
     */
    bool (*any)(struct collection*, bool (*any_func)(void *item));

    /*
     * Filter a collection. Returns a new collection with a copy of the data.
     */
    struct collection* (*filter)(struct collection*, bool (*filter_func)(void *item));

    /*
     * Call the provided function for each element in the collection.
     */
    void (*for_each)(struct collection*, void (*for_each_func)(void *next));

    /*
     * Map elements from a collection into a new collection.
     * The provided size is the size of the elements in the new collection.
     */
    struct collection* (*map)(struct collection*, void* (*map_func)(void *item), size_t size);

    /*
     * Reduce the collection to a single item.
     */
    void* (*reduce)(struct collection*, void* initial, void* (*reduce_func)(void* next, void* current), size_t size);

    /*
     * Clean up the collection. If free_func is not NULL, this function
     * will be called to deconstruct each individual element.
     */
    void (*cleanup)(struct collection *self, void (*free_func)(void *item));

    /*
     * Return the number of elements in the collection.
     */
    size_t (*count)(struct collection *self);

    void* (*at)(struct collection*, size_t i);

    size_t size;

    void *data;
} collection_t;

collection_t* new_array_list(size_t size);

#endif

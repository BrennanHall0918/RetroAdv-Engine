#include "collections.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct list {
    void *array;
    size_t item_size;
    size_t length;
    size_t array_size;
} list_t;

    /*
     * Append an item to the end of the collection.
     */
static void add(collection_t* self, void* item) {
    list_t* list = (list_t*)self->data;
    /*
    * IF adding this item to the end of this array would extend past the size of the array, reallocate (realloc)
    * the array to the size of the array * 2
    */
    if ((list->item_size * list->length) + list->item_size > list->array_size) {
        list = realloc(list, list->array_size * 2);

        list->array_size = list->array_size * 2;
    }

    char* u8_array = (char*)list->array;
    list->array = memcpy(u8_array + (list->item_size * list->length), item, list->item_size);
    list->length++;
}

    /*
     * Execute the provided function for each element in the collection,
     * return true if any_func returns true for any element.
     */
static bool any(bool (*any_func)(void *item)) {
    return false; //stub
}
    /*
     * Filter a collection. Returns a new collection with a copy of the data.
     */
static collection_t* filter(collection_t* self, bool (*filter_func)(void *item)) {
    collection_t* collection = new_array_list(self->size);
    list_t* list = (list_t*)self->data;
    for (int i = 0; i < list->length; i++) {
        if (filter_func(self->at(self, i))) {
            collection->add(collection, self->at(self, i));
        }
    }
    return collection; //stub
}

    /*
     * Call the provided function for each element in the collection.
     */
static void for_each(void (*for_each_func)(void *next)) {
    //stub
}

    /*
     * Map elements from a collection into a new collection.
     * The provided size is the size of the elements in the new collection.
     */
static collection_t* map(void* (*map_func)(void *item), size_t size) {
    return NULL; //stub
}

    /*
     * Reduce the collection to a single item.
     */
static void* reduce(void* initial, void* (*reduce_func)(void* next, void* current)) {
    return NULL; //stub
}

    /*
     * Clean up the collection. If free_func is not NULL, this function
     * will be called to deconstruct each individual element.
     */
static void cleanup(collection_t *self, void (*free_func)(void *item)) {
    //stub
}

    /*
     * Return the number of elements in the collection.
     */
static size_t count(collection_t *self) {
    return 0; //stub
}

static void* at(collection_t *self, size_t i) {
    list_t* list = (list_t*)self->data;
    char* u8_array = (char*)list->array;

    return (void*)(u8_array +i * list->item_size);
} 

collection_t* new_array_list(size_t size) {
    collection_t* collection = malloc(sizeof(collection_t));
    list_t* list = malloc(sizeof(list_t));

    list->array = malloc(size * 2);
    list->item_size = size;
    list->length = 0;
    list->array_size = size * 2;

    collection->data = list;
    collection->add = add;
    collection->any = any;
    collection->filter = filter;
    collection->for_each = for_each;
    collection->map = map;
    collection->reduce = reduce;
    collection->cleanup = cleanup;
    collection->count = count;
    collection->size = size;
    collection->at = at;

    return collection;
}
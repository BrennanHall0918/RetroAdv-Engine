#include "collections.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

typedef struct list {
    /*
     * array: the collection of elements
     */
    void *array;
    /*
     * item_size: size of each element
     */
    size_t item_size;
    /*
     * length: number of elements in the array
     */
    size_t length;
    /*
     * array_size: amount of memory dedicated to the array
     */
    size_t array_size;

    /*
     * element_cleanup: Free an element.
     */
    void (*element_cleanup)(void *item);
} list_t;

/*
 * Append an element to the end of the collection.
 */
static void add(collection_t* self, void* item) {
    list_t* list = (list_t*)self->data;
    /*
     * IF adding this element to the end of this array would extend past the size of the array, reallocate (realloc)
     * the array to the size of the array * 2
     */
    if ((list->item_size * list->length) + list->item_size > list->array_size) {
        size_t new_size = list->array_size * 2;
        list->array = realloc(list->array, new_size);
        list->array_size = new_size;
    }

    void* addr = list->array + (list->item_size * list->length);
    memcpy(addr, item, list->item_size);
    list->length++;
}

/*
 * Execute the provided function for each element in the collection (array),
 * return true if any_func returns true for any element.
 */
static bool any(collection_t* self, bool (*any_func)(void *item)) {
    list_t* list = (list_t*)self->data;
    for (int i = 0; i < list->length; i++) {
        if (any_func(self->at(self, i))) {
            return true;
        }
    }
    return false;
}
/*
 * Filter a collection. Returns a new collection with a copy of the data.
 */
static collection_t* filter(collection_t* self, bool (*filter_func)(void *item)) {
    list_t* list = (list_t*)self->data;
    collection_t* collection = new_array_list(self->size, list->element_cleanup);

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
static void for_each(collection_t* self, void (*for_each_func)(void *next)) {
    list_t* list = (list_t*)self->data;

    for (int i = 0; i < list->length; i++) {
        for_each_func(self->at(self, i));
    }
}

/*
 * Map elements from a collection into a new collection.
 * The provided size is the size of the elements in the new collection.
 */
static collection_t* map(collection_t* self, void* (*map_func)(void *item), size_t size) {
    list_t* list = (list_t*)self->data;
    collection_t* collection = new_array_list(size, NULL);

    for (int i = 0; i < list->length; i++) {
        void* new_item = map_func(self->at(self, i));
        collection->add(collection, new_item);
        free(new_item);
    }

    return collection;
}

/*
 * Reduce the collection to a single item.
 */
static void* reduce(collection_t* self, void* initial, void* (*reduce_func)(void* next, void* current), size_t size) {
    list_t* list = (list_t*)self->data;
    void* result = malloc(size);
    memcpy(result, initial, size);

    for (int i = 0; i < list->length; i++) {
        void* temp = reduce_func(self->at(self, i), result);
        result = memcpy(result, temp, size);
        free(temp);
    }

    return result;
}

/*
 * Clean up the collection. If free_func is not NULL, this function
 * will be called to deconstruct each individual element.
 */
static void cleanup(collection_t **self) {
    list_t* list = (list_t*)(*self)->data;
    if (list->element_cleanup != NULL) {
        (*self)->for_each(*self, list->element_cleanup);
    }

    free(list->array);
    free((*self)->data);
    free(*self);
}

/*
 * Return the number of elements in the collection.
 */
static size_t count(collection_t *self) {
    list_t* list = (list_t*)self->data;
    return list->length;
}

static void set(collection_t *self, void* value, size_t i) {
    list_t* list = (list_t*)self->data;
    void* item = self->at(self, i);

    if (list->element_cleanup != NULL) {
        list->element_cleanup(item);
    }

    memcpy(item, value, list->item_size);
} 

static void* at(collection_t *self, size_t i) {
    list_t* list = (list_t*)self->data;

    return (void*)(list->array + (i * list->item_size));
} 

collection_t* new_array_list(size_t size, void (*cleanup_func)(void *item)) {
    collection_t* collection = malloc(sizeof(collection_t));
    list_t* list = malloc(sizeof(list_t));

    list->array = malloc(size * 2);
    list->item_size = size;
    list->length = 0;
    list->array_size = size * 2;
    list->element_cleanup = cleanup_func;

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
    collection->set = set;

    return collection;
}

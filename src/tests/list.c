#define _GNU_SOURCE
#include "collections.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "testing.h"

typedef struct {
    size_t len;
    char *str;
} string_t;

string_t* init_string(char *s) {
    string_t* str = malloc(sizeof(string_t));
    str->len = strlen(s);
    str->str = malloc(str->len + 1); // include '\0'
    memcpy(str->str, s, str->len + 1);
    return str;
}

void cleanup_string_element(void *item) {
    string_t* str = (string_t*)item;
    free(str->str);
}


bool test_init_array(void) {
    collection_t* collection = new_array_list(sizeof(int), NULL);

    if (collection == NULL) {
        return false;
    }

    collection->cleanup(&collection);
    return true;
}

bool test_add_at(void) {
    collection_t* collection = new_array_list(sizeof(int), NULL);

    for (int i = 0; i < 10; i++) {
        collection->add(collection, &i);
        void *item = collection->at(collection, i);
        if (*(int*)item != i) {
            return false;
        }
    }

    collection->cleanup(&collection);
    return true;
}

bool test_add_count(void) {
    collection_t* collection = new_array_list(sizeof(int), NULL);
    int i = 1;
    collection->add(collection, (void*)(&i));
    collection->add(collection, (void*)(&i));
    collection->add(collection, (void*)(&i));
    collection->add(collection, (void*)(&i));

    if (collection->count(collection)!= 4) {
        return false;
    } 

    collection->cleanup(&collection);
    return true;
}

bool filter(void* item) {
    int* i = (int*)item;

    if (*i % 2 == 0) {
        return true;
    }

    return false;
}

bool test_add_filter(void) {
    collection_t* collection = new_array_list(sizeof(int), NULL);

    for (int i = 1; i <= 100; i++ ) {
        collection->add(collection, (void*)(&i));
    }

    collection_t* filtered_collection = collection->filter(collection, filter);

    if (filtered_collection->count(filtered_collection) == 50) {
        filtered_collection->cleanup(&filtered_collection);
        collection->cleanup(&collection);
        return true;
    }

    return false;
    
}

bool any1(void* element) {
    int* i = (int*)element;
    return *i == 1;
}

bool any2(void* element) {
    int* i = (int*)element;
    return *i == 2;
}

bool test_add_any(void) {
    collection_t* collection = new_array_list(sizeof(int), NULL);
    int i = 1;
    collection->add(collection, (void*)(&i));
    collection->add(collection, (void*)(&i));
    collection->add(collection, (void*)(&i));
    collection->add(collection, (void*)(&i));

    if (!(collection->any(collection, any1))) {
        return false;
    }

    if (collection->any(collection, any2)) {
        return false;
    }

    collection->cleanup(&collection);
    return true;
}

void* map_square(void* item) {
    int* i = (int*)item;
    int* result = malloc(sizeof(int));
    *result = *i * *i;

    return (void*)result;
}

bool test_add_map(void) {
    collection_t* collection = new_array_list(sizeof(int), NULL);

    for (int i = 1; i <= 10; i++) {
        collection->add(collection, (void*)(&i));
    }

    collection_t* squared_collection = collection->map(collection, map_square, sizeof(int));

    for (int i = 0; i < collection->count(collection); i++) {
        int n = *(int*)(collection->at(collection, i));
        if (*(int*)(squared_collection->at(squared_collection, i)) != n * n) {
            return false;
        }
    }

    squared_collection->cleanup(&squared_collection);
    collection->cleanup(&collection);
    return true;
}

void* sum(void* next,void* current) {
    int* result = malloc(sizeof(int));
    *result = *(int*)next + *(int*)current;

    return (void*)result;
}

bool test_add_reduce(void) {
    collection_t* collection = new_array_list(sizeof(int), NULL);

    for (int i = 1; i <= 10; i++) {
        collection->add(collection, (void*)(&i));
    }

    int initial = 0;

    int* result = collection->reduce(collection, &initial, sum, sizeof(int));

    if (*result != 55) {
        return false;
    }

    collection->cleanup(&collection);
    return true;
}

bool test_set(void)
{
    collection_t* c = new_array_list(sizeof(string_t), cleanup_string_element);
    for (int i = 0; i < 10000; i++)
    {
        char *s;
        asprintf(&s, "test%d\n", i);
        string_t* str = init_string(s);
        c->add(c, str);

        free(str);
        free(s);
    }

    char test[] = "Let's change this to something else";

    string_t* str = init_string(test);
    c->set(c, str, 921);
    free(str);
    str = c->at(c, 921);

    if (strcmp(str->str, test) == 0)
    {
        c->cleanup(&c);
        return true;
    }

    return false;
}

bool run_test(char* func_name, bool (*func)(void)) {
    printf("%s: ",func_name);

    if (!func()) {
        printf("Failed\n");

        return false;
    }

    printf("Succeeded\n");

    return true;
}
 
int main(void) {
    int failed_test = 0;

    if (!run_test("test_init_array", test_init_array)) {
        failed_test++;
    }

    if (!run_test("test_add_at", test_add_at)) {
        failed_test++;
    }

    if (!run_test("test_add_count", test_add_count)) {
        failed_test++;
    }

    if (!run_test("test_add_filter", test_add_filter)) {
        failed_test++;
    }

    if (!run_test("test_add_any", test_add_any)) {
        failed_test++;
    }

    if (!run_test("test_add_map", test_add_map)) {
        failed_test++;
    }

    if (!run_test("test_set", test_set)) {
        failed_test++;
    }

    return failed_test;
}

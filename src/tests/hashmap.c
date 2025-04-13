#include "collections.h"
#define _GNU_SOURCE
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "testing.h"

bool test_hash(void) {
    char str[] = "Testing";
    hashable_t* hashable = hashable_init((uint8_t*)str, strlen(str));
    hashable_t* hashable_2 = hashable_init((uint8_t*)str, strlen(str));

    return hashable->hash(hashable) == hashable_2->hash(hashable_2);
}

typedef struct value {
    char* str;
    uint64_t value;
} value_t;

bool test_hashmap_get_set(void) {
    value_t* values = malloc(sizeof(value_t) * 1000);
    hash_map_t* hash_map = hash_map_init(sizeof(value_t));

    for(int i = 0; i < 1000; i++) {
        asprintf(&(values[i].str), "Test%d", i);
        values[i].value = i;
        hashable_t* hashable = hashable_init((uint8_t*)values[i].str, strlen(values[i].str));
        hash_map_set(hash_map, hashable, &(values[i].value)); 
    }

    hashable_t* hashable = hashable_init((uint8_t*)values[10].str, strlen(values[10].str));
    uint64_t* value = (uint64_t*)hash_map_get(hash_map, hashable);

    return *value == 9;
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

    if (!run_test("test_hash", test_hash)) {
        failed_test++;
    }

    if (!run_test("test_hashmap_get_set", test_hashmap_get_set)) {
        failed_test++;
    }

    return failed_test;
}
#include "collections.h"
#include <stdio.h>
#include <stdbool.h>
#include <windows.h>

bool test_init_array(void) {
    collection_t* collections = new_array_list(sizeof(int));

    if (collections == NULL) {
        return false;
    }

    return true;
}

bool test_add_at(void) {
    collection_t* collections = new_array_list(sizeof(int));

    for (int i = 0; i < 10; i++) {
        collections->add(collections, (void*)(&i));
        if (*(int*)(collections->at(collections, i)) != i) {
            return false;
        }
    }
    return true;
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
 
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
    int failed_test = 0;

    if (!run_test("test_init_array", test_init_array)) {
        failed_test++;
    }

    if (!run_test("test_add_at", test_add_at)) {
        failed_test++;
    }

    return failed_test;
}

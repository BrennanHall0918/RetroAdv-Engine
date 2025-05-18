#include "repo.h"
#include <stdio.h>
#include <stdlib.h>

sqlite3* init_db(void) {
    sqlite3* db;
    int return_code;

    return_code = sqlite3_open("database.sqlite", &db);
    if(return_code) {
        fprintf(stderr, "Can't open Databse: %s\n", sqlite3_errmsg(db));
        exit(return_code);
    }

    return db;
}


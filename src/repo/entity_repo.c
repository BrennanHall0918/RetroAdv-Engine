#include "repo.h"
#include "collections.h"
#include "entity.h"
#include <string.h>
#include <stdlib.h>

// static void cleanup_tags(void* item) {
//     free(item);

// }

static int get_entity_types_callback(void* data, int n_columns, char** values, char** columns) {
    hash_map_t* map = (hash_map_t*)data;
    entity_type_t entity;
    // init_entity(&entity);

    for (size_t i = 0; i<n_columns; i++) {
        char* column = columns[i];

        if(strcmp(column, "maximum_speed") == 0) {
            entity.maximum_speed = strtod(values[i], NULL);
        } else if(strcmp(column, "acceleration") == 0) {
            entity.acceleration = strtod(values[i], NULL);
        } else if(strcmp(column, "width") == 0) {
            entity.width = atoi(values[i]);
        } else if(strcmp(column, "height") == 0) {
            entity.height = atoi(values[i]);
        } else if(strcmp(column, "name") == 0) {
            size_t len = strlen(values[i]) + 1;
            char* str = malloc(len);
            memcpy(str, values[i], len);
            entity.entity_type = str;
        }
    }


    hashable_t* hashable = hashable_init(entity.entity_type, strlen(entity.entity_type  ) + 1);
    hash_map_set(map, hashable, &entity);
    hashable->destroy(&hashable);

    return 0;
}

hash_map_t* get_entity_types(void) {
    hash_map_t* map = hash_map_init(sizeof(entity_t));
    sqlite3* db = init_db();
    sqlite3_exec(
        db,
        "SELECT"
        "    maximum_speed,"
        "    acceleration,"
        "    width,"
        "    height,"
        "    name"
        "FROM entity;",
        get_entity_types_callback,
        map,
        NULL
    );

    return map;
}
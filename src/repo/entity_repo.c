#define _GNU_SOURCE
#include "repo.h"
#include "collections.h"
#include "entity.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// static void cleanup_tags(void* item) {
//     free(item);

// }

static entity_tag_t entity_populate(void* data, int n_columns, char** values, char** columns) {
    entity_tag_t tag;

    for (size_t i = 0; i<n_columns; i++) {
        char* column = columns[i];
         
        if(strcmp(column, "tag") == 0) {
            size_t len = strlen(values[i]) + 1;
            char* str = malloc(len);
            memcpy(str, values[i], len);
            tag.entity_tag = str;
        } else if (strcmp(column, "duration") == 0) {
            tag.duration = atoi(values[i]);
        } else if (strcmp(column, "entity_id") == 0) {
            // TODO: Change query to pull entity type name for hashable.
            // hashable_t* hashable = hashable_init(atoi(values[i]))
            // tag.entity_id = atoi(values[i]); 
        } else if (strcmp(column, "id") == 0) {
            tag.tag_id = atoi(values[i]);
        }
    }

    return tag;
}

static int get_entity_tags_list_callback(void* data, int n_columns, char** values, char** columns) {
    collection_t* list = (collection_t*)data;
    entity_tag_t tag = entity_populate(data, n_columns, values, columns);
    list->add(list, &tag);

    return 0;
}

static int get_entity_tags_hash_map_callback(void* data, int n_columns, char** values, char** columns) {
    hash_map_t* map = (hash_map_t*)data;
    entity_tag_t tag = entity_populate(data, n_columns, values, columns);
    hashable_t* hashable = hashable_init(tag.entity_tag, strlen(tag.entity_tag));
    hash_map_set(map, hashable, &tag);
    

    return 0;
}

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
        } else if(strcmp(column, "id") == 0) {
            entity.entity_id = atoi(values[i]);
        }
    }


    hashable_t* hashable = hashable_init(entity.entity_type, strlen(entity.entity_type  ) + 1);
    hash_map_set(map, hashable, &entity);
    hashable->destroy(&hashable);

    return 0;
}

collection_t* get_entity_type_tags_by_entity(int id) {
    char* sql;
    collection_t* map = new_array_list(sizeof(entity_tag_t), NULL);
    sqlite3* db = init_db();

    int status = asprintf(
        &sql, 
        "SELECT\n" 
	    "    t.id,\n"
        "    t.tag,\n"
        "    t.entity_id,\n"
        "    e.name\n"
        "    FROM entity_type_tag t\n"
        "    INNER JOIN entity_type e ON e.id = t.entity_id\n"
        "    WHERE e.id = %d;",
        id
    );

    if (status == -1) {
        fputs("entity_repo.c: get_entity_type_tags_by_entity: could not allocate string.", stderr);
        exit(1);
    }

    sqlite3_exec(
        db,
        sql,
        get_entity_tags_list_callback,
        map,
        NULL
    );

    return map;
}

hash_map_t* get_entity_type_tags(void) {
    hash_map_t* map = hash_map_init(sizeof(entity_tag_t));

    sqlite3* db = init_db();
    sqlite3_exec(
        db,
        "SELECT\n"
	    "    t.id,\n"
        "    t.tag,\n"
        "    t.entity_id,\n"
        "    e.name\n"
        "FROM entity_type_tag t\n"
        "INNER JOIN entity_type e ON e.id = t.entity_id;",
        get_entity_tags_hash_map_callback,
        map,
        NULL
    );

    return map;
}

hash_map_t* get_entity_types(void) {
    hash_map_t* map = hash_map_init(sizeof(entity_t));
    sqlite3* db = init_db();
    sqlite3_exec(
        db,
        "SELECT\n"
        "    maximum_speed,\n"
        "    acceleration,\n"
        "    width,\n"
        "    height,\n"
        "    name\n"
        "FROM entity_type;",
        get_entity_types_callback,
        map,
        NULL
    );

    return map;
}

static int pull_sprite_set_callback(void* data, int n_columns, char** values, char** columns) {
    entity_sprite_set_t* sprite_set = (entity_sprite_set_t*)data;

    for (size_t i = 0; i<n_columns; i++) {
        char* column = columns[i];

        if(strcmp(column, "entity_type_id") == 0) {
            sprite_set->entity_type_id = atoi(values[i]);
        } else if (strcmp(column, "sprite_sheet_id") == 0) {
            sprite_set->sprite_sheet_id = atoi(values[i]);
        } else if (strcmp(column, "tag_id") == 0) {
            sprite_set->tag_id = atoi(values[i]);
        } else if (strcmp(column, "offset_x") == 0) {
            sprite_set->offset_x = atoi(values[i]);
        } else if (strcmp(column, "offset_y") == 0) {
            sprite_set->offset_y = atoi(values[i]);
        } else if (strcmp(column, "sprite_width") == 0) {
            sprite_set->sprite_width= atoi(values[i]);
        } else if (strcmp(column, "sprite_height") == 0) {
            sprite_set->sprite_height = atoi(values[i]);
        } else if (strcmp(column, "stride") == 0) {
            sprite_set->stride = atoi(values[i]);
        } else if (strcmp(column, "frames") == 0) {
            sprite_set->frames = atoi(values[i]);
        } else if(strcmp(column, "path") == 0) {
            size_t len = strlen(values[i]) + 1;
            char* str = malloc(len);
            memcpy(str, values[i], len);
            sprite_set->path = str;
        }

    }

    return 0;
}

entity_sprite_set_t* pull_sprite_set(int entity_type_id) {
    entity_sprite_set_t* sprite_set = malloc(sizeof(entity_sprite_set_t));
    char* sql;

    sqlite3* db = init_db();
    int status = asprintf(
        &sql,
        "SELECT\n"
	    "`set`.entity_type_id,\n"
        "`set`.sprite_sheet_id,\n"
        "`set`.tag_id,\n"
        "`set`.offset_x,\n"
        "`set`.offset_y,\n"
        "`set`.sprite_width,\n"
        "`set`.sprite_height,\n"
        "`set`.stride,\n"
        "`set`.frames,\n"
        "sheet.path\n"
        "FROM entity_sprite_set `set`\n"
        "INNER JOIN sprite_sheet sheet ON sheet.id = `set`.sprite_sheet_id\n" 
        "WHERE `set`.entity_type_id = %d",
        entity_type_id
    );

    if (status == -1) {
        fputs("entity_repo.c: pull_sprite_set: could not allocate string.", stderr);
        exit(1);
    }

    sqlite3_exec(
        db,
        sql,
        pull_sprite_set_callback,
        sprite_set,
        NULL
    );

    return sprite_set;
}

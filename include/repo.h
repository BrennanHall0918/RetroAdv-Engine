#ifndef _REPO_H_
#define _REPO_H_

#include <sqlite3.h>
#include "collections.h"
#include "entity.h"

sqlite3* init_db(void);
hash_map_t* get_entity_types(void);
collection_t* get_entity_type_tags_by_entity(int id);
hash_map_t* get_entity_type_tags(void);
entity_sprite_set_t* pull_sprite_set(int entity_type_id);

#endif

#ifndef _REPO_H_
#define _REPO_H_

#include <sqlite3.h>
#include "collections.h"

sqlite3* init_db(void);
hash_map_t* get_entity_types(void);

#endif

#ifndef _app_h_
#define _app_h_

#include <SDL3/SDL.h>
#include "entity.h"
#include "collections.h"
#include <inttypes.h>
#include <sqlite3.h>

typedef struct {
	SDL_Window *window;

	SDL_Renderer *renderer;

	SDL_Texture *background;
	SDL_Texture *player_texture;
	SDL_Texture *grass_tile;

	SDL_FRect player_sprite_offset_rect;
	SDL_FRect player_location;

	SDL_IOStreamInterface iface;

	collection_t* entities;
	hash_map_t* entity_sprite_sets;
	hash_map_t* sprite_sheets;
	hash_map_t* entity_tags;
	hash_map_t* entity_types;

	uint64_t last_tick;
	uint64_t current_tick;
	float delta_time;

} app_t;

/*
	Logs an error.
*/
void log_error(char *prefix);

/*
	Clamp a value within a range.
*/
float clamp (float n, float min, float max);

#endif

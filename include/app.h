#ifndef _app_h_
#define _app_h_

#include <SDL3/SDL.h>
#include "entity.h"
#include <inttypes.h>

#define N_ENTITIES 1

typedef struct {
	SDL_Window *window;

	SDL_Renderer *renderer;

	SDL_Texture *background;
	SDL_Texture *player_texture;

	SDL_FRect player_sprite_offset_rect;
	SDL_FRect player_location;

	SDL_IOStreamInterface iface;

	entity_t* entities;

	uint64_t last_tick;
	uint64_t current_tick;
	float delta_time;
} app_t;

void log_error(char *prefix);

#endif

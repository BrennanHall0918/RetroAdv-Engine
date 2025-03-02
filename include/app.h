#ifndef _app_h_
#define _app_h_

#include <SDL3/SDL.h>
#include "entity.h"

#define N_ENTITIES 2

typedef struct {
	SDL_Window *window;

	SDL_Renderer *renderer;

	SDL_Texture *background;
	SDL_Texture *player_texture;

	SDL_FRect player_sprite_offset_rect;
	SDL_FRect player_location;

	SDL_IOStreamInterface iface;

	entity_t* entities;
} app_t;

void log_error(char *prefix);

#endif

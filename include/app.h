#ifndef _app_h_
#define _app_h_

#include <SDL3/SDL.h>

typedef struct {
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Texture *player_texture;
} app_t;

#endif

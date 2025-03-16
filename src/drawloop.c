#include "app.h"
#include "entity.h"

#include <stdio.h>
#include <SDL3/SDL.h>

void render(app_t* state)
{
	SDL_RenderClear(state->renderer);
	SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, 255);

	SDL_RenderTextureTiled(state->renderer, state->grass_tile, NULL, 1, NULL);

	for (int i = 0; i < N_ENTITIES; i++) 
	{
		state->entities[i].render((void*)(state->entities+i), state->renderer);
	}

	SDL_RenderPresent(state->renderer);

	SDL_Delay(17);
}

SDL_AppResult SDL_AppIterate(void *appstate, SDL_Event *event)
{
	app_t* state = (app_t*) appstate;
	state->last_tick = state->current_tick;
	state->current_tick = SDL_GetTicks();
	state->delta_time = (state->current_tick - state->last_tick) / 1000.0f;
	render(state);

	for (int i = 0; i < N_ENTITIES; i++) 
	{
		state->entities[i].handle_events((void*)(state->entities+i), event);
	}

	for (int i = 0; i < N_ENTITIES; i++) 
	{
		state->entities[i].update((void*)(state->entities+i), state->delta_time);
	}

	return SDL_APP_CONTINUE;
}

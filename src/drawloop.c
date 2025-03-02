#include "app.h"
#include "entity.h"

#include <stdio.h>

void render(app_t* state)
{
	SDL_RenderClear(state->renderer);
	SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, 255);

	for (int i = 0; i < N_ENTITIES; i++) 
	{
		state->entities[i].render((void*)(state->entities+i), state->renderer);
	}

	SDL_RenderPresent(state->renderer);
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
	app_t* state = (app_t*) appstate;
	state->last_tick = state->current_tick;
	state->current_tick = SDL_GetTicks();
	state->delta_time = (state->current_tick - state->last_tick) / 1000.0f;
	printf("%f\n", state->delta_time);
	render(state);
	return SDL_APP_CONTINUE;
}

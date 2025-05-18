#include "app.h"
#include "entity.h"

#include <stdio.h>
#include <SDL3/SDL.h>

void render(app_t* state)
{
	SDL_RenderClear(state->renderer);
	SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, 255);

	SDL_RenderTextureTiled(state->renderer, state->grass_tile, NULL, 1, NULL);

	for (size_t i = 0; i < state->entities->count(state->entities); i++) 
	{
		entity_t* entity = state->entities->at(state->entities, i);
		entity->render(entity, state->renderer);
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

	for (size_t i = 0; i < state->entities->count(state->entities); i++) 
	{
		entity_t* entity = state->entities->at(state->entities, i);
		entity->handle_events(entity, event);
	}

	for (size_t i = 0; i < state->entities->count(state->entities); i++) 
	{
		entity_t* entity = state->entities->at(state->entities, i);
		entity->update(entity, state->delta_time);
	}

	return SDL_APP_CONTINUE;
}

#include "app.h"
#include "entity.h"
#include <SDL3/SDL.h>

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
	app_t* state = (app_t*)appstate;

	switch (event->type)
	{
		case SDL_EVENT_QUIT:
			return SDL_APP_SUCCESS;
	}

	for (int i = 0; i < N_ENTITIES; i++) 
	{
		state->entities[i].handle_events((void*)(state->entities+i), event);
	}

	for (int i = 0; i < N_ENTITIES; i++) 
	{
		state->entities[i].update((void*)(state->entities+i));
	}

	return SDL_APP_CONTINUE;
}
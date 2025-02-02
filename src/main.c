#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>

#include <stdlib.h>
#include <stdio.h>

#include "app.h"

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
	app_t* state = malloc(sizeof(app_t));
	*appstate = state;

	if (!SDL_InitSubSystem(SDL_INIT_VIDEO|SDL_INIT_EVENTS))
	{
		SDL_Log("%s\n", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	state->window = SDL_CreateWindow(
			"Retro ADV",
			800,
			600,
			0);

	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
	switch (event->type)
	{
		case SDL_EVENT_QUIT:
			return SDL_APP_SUCCESS;
	}

	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
	app_t *state = (app_t *)appstate;
	SDL_DestroyWindow(state->window);
	free(appstate);
	return;
}

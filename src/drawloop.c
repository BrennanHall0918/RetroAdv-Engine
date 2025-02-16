#include "app.h"



void render(app_t* state)
{
	SDL_RenderClear(state->renderer);
	SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, 255);
	SDL_RenderTexture(state->renderer, state->player_texture, NULL, NULL);

	SDL_RenderPresent(state->renderer);
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
	app_t* state = (app_t*) appstate;
	render(state);
	return SDL_APP_CONTINUE;
}


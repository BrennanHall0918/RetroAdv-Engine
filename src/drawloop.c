#include "app.h"
#include "entity.h"



void render(app_t* state)
{
	SDL_RenderClear(state->renderer);
	SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, 255);

	for (int i = 0; i < N_ENTITIES; i++) 
	{
		state->entities[i].render((void*)(state->entities+i), state->renderer);
	}


/*
	state->player_sprite_offset_rect = create_rect(18, 17, 13, 15);
	state->player_location 	= create_rect(100, 100, 13, 15);
	SDL_SetTextureScaleMode(state->player_texture, SDL_SCALEMODE_NEAREST); 
	SDL_RenderTexture(state->renderer, state->player_texture, &(state->player_sprite_offset_rect), &(state->player_location));
*/

	SDL_RenderPresent(state->renderer);
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
	app_t* state = (app_t*) appstate;
	render(state);
	return SDL_APP_CONTINUE;
}

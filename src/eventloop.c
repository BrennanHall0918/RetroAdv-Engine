#include "app.h"
#include "entity.h"
#include <SDL3/SDL.h>

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{

	switch (event->type)
	{
		case SDL_EVENT_QUIT:
			return SDL_APP_SUCCESS;
	}

	return SDL_APP_CONTINUE;
}
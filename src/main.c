#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>

#include <stdio.h>
#include <stdlib.h>

#include "app.h"
#include "entity.h"

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
    app_t *state = malloc(sizeof(app_t));
    *appstate = state;
    state->entities = malloc(sizeof(entity_t)*N_ENTITIES);

    if (!SDL_InitSubSystem(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
        SDL_Log("%s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    state->window = SDL_CreateWindow("Retro ADV", 800, 600, 0);

    if (!state->window) {
        log_error("Error initializing window");
        return SDL_APP_FAILURE;
    }

    state->renderer = SDL_CreateRenderer(state->window, NULL);

    if (!state->renderer) {
        log_error("Error loading renderer");
        return SDL_APP_FAILURE;
    }

    if (!init_player(&(state->entities[0]), state->renderer, "./char_spritesheet.png"))
    {
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    app_t *state = (app_t *)appstate;

    for (int i = 0; i < N_ENTITIES; i++)
    {
        state->entities[i].destroy((void*)(state->entities+i));
    }

    free(state->entities);
    SDL_DestroyWindow(state->window);
    free(appstate);
    return;
}

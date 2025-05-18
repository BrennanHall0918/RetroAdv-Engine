#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>

#include <stdio.h>
#include <stdlib.h>

#include "app.h"
#include "entity.h"
#include "repo.h"

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
    app_t *state = malloc(sizeof(app_t));
    *appstate = state;
    state->entities = new_array_list(sizeof(entity_t), NULL);

    state->last_tick = 0;
    state->current_tick = 0;

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

    state->grass_tile = IMG_LoadTexture(state->renderer, "Grass_tile.png");

    if (!state->grass_tile) {
        log_error("Error getting texture");
        return false;
    }

    state->entity_types = get_entity_types();
    state->entity_tags = get_entity_type_tags();

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    app_t *state = (app_t *)appstate;

    for (size_t i = 0; i < state->entities->count(state->entities); i++)
    {
        entity_t* entity = state->entities->at(state->entities, i);
        entity->destroy(entity);
    }

    free(state->entities);
    SDL_DestroyWindow(state->window);
    free(appstate);
    return;
}

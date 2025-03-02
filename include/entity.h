#ifndef _entity_h_
#define _entity_h_

#include <SDL3/SDL.h>

typedef struct {
    void (*destroy)(void*);
    void (*handle_events)(void*, SDL_Event*);
    void (*update)(void*, float delta_time);
    void (*render)(void*, SDL_Renderer*);
    float speed;

    SDL_Texture *texture;

    SDL_FRect offset;
    SDL_FRect location;
} entity_t;

bool init_player(entity_t* entity, SDL_Renderer* renderer, char* path);

#endif
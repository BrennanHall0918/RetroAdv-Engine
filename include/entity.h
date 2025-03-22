#ifndef _entity_h_
#define _entity_h_

#include <SDL3/SDL.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600


typedef struct {
    void (*destroy)(void*);
    void (*handle_events)(void*, SDL_Event*);
    void (*update)(void*, float delta_time);
    void (*render)(void*, SDL_Renderer*);

    float maximum_speed;
    float acceleration;
    float velocity_x;
    float velocity_y;

    SDL_Texture *texture;

    SDL_FRect offset;
    SDL_FRect location;
} entity_t;

bool init_entity(entity_t* entity, SDL_Renderer* renderer, char* path);

#endif
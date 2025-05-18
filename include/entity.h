#ifndef _entity_h_
#define _entity_h_

#include <SDL3/SDL.h>
#include "collections.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600


typedef struct {
    void (*destroy)(void*);
    void (*handle_events)(void*, SDL_Event*);
    void (*update)(void*, float delta_time);
    void (*render)(void*, SDL_Renderer*);
    void (*load_texture) (char* path, SDL_Renderer* renderer);

    double maximum_speed;
    double acceleration;
    double velocity_x;
    double velocity_y;
    int width;
    int height;

    hashable_t* sprite_sheet_name;
    hashable_t* entity_type;

    collection_t *tags;

    SDL_Texture *texture;

    SDL_FRect offset;
    SDL_FRect location;
} entity_t;

typedef struct {
    int entity_id;
    char* entity_type;

    double maximum_speed;
    double acceleration;

    int width;
    int height;

    collection_t *sprite_sheets;
    collection_t *tags;
} entity_type_t;

typedef struct{
    int tag_id;
    char* tag;

    int duration;
    int timestamp;
} tag_t;

typedef struct {
    int sprite_sheet_id;
    char* path;
    char* name;

    int width;
    int height;
} sprite_sheet_t;

typedef struct {
    int entity_sprite_set_id;
    hashable_t *sprite_sheet_hashable;

    int entity_id;
    int sprite_sheet_id;
    int tag_id;

    int offset_x;
    int offset_y;
    int sprite_width;
    int sprite_height;
    int stride;
} entity_sprite_set_t;



bool init_entity(entity_t* entity, SDL_Renderer* renderer, char* path);

#endif
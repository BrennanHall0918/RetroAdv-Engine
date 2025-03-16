#include "app.h"
#include "entity.h"
#include <SDL3_image/SDL_image.h>
#include <stdio.h>

static void destroy(void* self)
{

}

static void handle_events(void* self, SDL_Event* event)
{

}

static void update_accelerate(entity_t* entity, float delta_time, const bool *keyboard_state)
{
    if (keyboard_state[SDL_SCANCODE_UP])
    {
        printf("location: (%f, %f)\n", entity->location.x, entity->location.y);
        entity->velocity_y = clamp(entity->velocity_y - entity->acceleration, entity->maximum_speed * -1, entity->maximum_speed); 
        printf("velocity: (%f, %f)\n", entity->velocity_x, entity->velocity_y);
        entity->location.y = clamp(entity->location.y + entity->velocity_y * delta_time,  0, SCREEN_HEIGHT);
        printf("location: (%f, %f)\n", entity->location.x, entity->location.y);
        }

    if (keyboard_state[SDL_SCANCODE_DOWN])
    {
        printf("location: (%f, %f)\n", entity->location.x, entity->location.y);
        entity->velocity_y = clamp(entity->velocity_y + entity->acceleration, entity->maximum_speed * -1, entity->maximum_speed);
        printf("velocity: (%f, %f)\n", entity->velocity_x, entity->velocity_y);
        entity->location.y = clamp(entity->location.y + entity->velocity_y * delta_time, 0, SCREEN_HEIGHT);
        printf("location: (%f, %f)\n", entity->location.x, entity->location.y);
    }

    if (keyboard_state[SDL_SCANCODE_LEFT])
    {
        printf("location: (%f, %f)\n", entity->location.x, entity->location.y);
        entity->velocity_x = clamp(entity->velocity_x - entity->acceleration, entity->maximum_speed * -1, entity->maximum_speed);
        printf("velocity: (%f, %f)\n", entity->velocity_x, entity->velocity_y);
        entity->location.x = clamp(entity->location.x + entity->velocity_x *delta_time, 0, SCREEN_WIDTH);
        printf("location: (%f, %f)\n", entity->location.x, entity->location.y);
    }

    if (keyboard_state[SDL_SCANCODE_RIGHT])
    {
        printf("location: (%f, %f)\n", entity->location.x, entity->location.y);
        entity->velocity_x = clamp(entity->velocity_x + entity->acceleration, entity->maximum_speed * -1, entity->maximum_speed);
        printf("velocity: (%f, %f)\n", entity->velocity_x, entity->velocity_y);
        entity->location.x = clamp(entity->location.x + entity->velocity_x *delta_time, 0, SCREEN_WIDTH);
        printf("location: (%f, %f)\n", entity->location.x, entity->location.y);
    }
}

static void update_decelerate(entity_t* entity, float delta_time, const bool *keyboard_state)
{
    if (!keyboard_state[SDL_SCANCODE_UP] && !keyboard_state[SDL_SCANCODE_DOWN]) {
        if (entity->velocity_y < 0) {
            entity->velocity_y = clamp(entity->velocity_y + 10, entity->maximum_speed * -1, 0);
        }
        else if (entity->velocity_y > 0) {
            entity->velocity_y = clamp(entity->velocity_y - 10, 0, entity->maximum_speed);
        }
    }

    if (!keyboard_state[SDL_SCANCODE_LEFT] && !keyboard_state[SDL_SCANCODE_RIGHT]) {
        if (entity->velocity_x < 0) {
            entity->velocity_x = clamp(entity->velocity_x + 10, entity->maximum_speed * -1, 0);
        }
        else if (entity->velocity_x > 0) {
            entity->velocity_x = clamp(entity->velocity_x - 10, 0, entity->maximum_speed);
        }
    }
}

static void update(void* self, float delta_time)
{
    const bool *keyboard_state = SDL_GetKeyboardState(NULL);

    entity_t* entity = (entity_t*)self;

    update_accelerate(entity, delta_time, keyboard_state);
    update_decelerate(entity, delta_time, keyboard_state);
}

static void render_entity(void* self, SDL_Renderer* renderer)
{
    entity_t* entity = (entity_t*)self;
    SDL_RenderTexture(renderer, entity->texture, &(entity->offset), &(entity->location));
}

static SDL_FRect create_rect(float x, float y, float w, float h)
{
	SDL_FRect rect = {x, y, w, h};
	return rect;
} 

bool init_entity (entity_t* entity, SDL_Renderer* renderer, char* path)
{
    entity->velocity_x = 0;
    entity->velocity_y = 0;
    entity->maximum_speed = 100;
    entity->acceleration = 5;
    entity->destroy = &destroy;
    entity->handle_events = &handle_events;
    entity->update = &update;
    entity->render = &render_entity;
    
    entity->texture = IMG_LoadTexture(renderer, "char_spritesheet.png");

    if (!entity->texture) {
        log_error("Error getting texture");
        return false;
    }

    entity->offset = create_rect(18, 17, 13, 15);
	entity->location = create_rect(100, 100, 13, 15);
	SDL_SetTextureScaleMode(entity->texture, SDL_SCALEMODE_NEAREST); 
	
    return true;
}
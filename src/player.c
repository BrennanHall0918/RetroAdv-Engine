#include "app.h"
#include "entity.h"
#include <SDL3_image/SDL_image.h>

void destroy(void* self)
{

}
void handle_events(void* self, SDL_Event* event)
{

}
void update(void* self)
{

}
void render_player(void* self, SDL_Renderer* renderer)
{
    entity_t* entity = (entity_t*)self;
    SDL_RenderTexture(renderer, entity->texture, &(entity->offset), &(entity->location));
}

SDL_FRect create_rect(float x, float y, float w, float h)
{
	SDL_FRect rect = {x, y, w, h};
	return rect;
} 

bool init_player (entity_t* entity, SDL_Renderer* renderer, char* path)
{
    entity->destroy = &destroy;
    entity->handle_events = &handle_events;
    entity->update = &update;
    entity->render = &render_player;
    
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
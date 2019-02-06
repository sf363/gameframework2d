#include "player.h"
#include "simple_logger.h"

void player_update(Entity *self);

Entity *player_new(Vector2D position)
{
    Entity *player = NULL;
    
    player = entity_new();
    if (!player)
    {
        slog("failed to allocate a new player entity");
        return NULL;
    }
    vector2d_copy(player->position,position);
    
    player->sprite = gf2d_sprite_load_all("images/ed210.png",128,128,16);
    player->frame = 48;
    player->update = player_update;
    return player;
}

void player_update(Entity *self)
{
    const Uint8 *keys;
    // I do stuff derp derp
    keys = SDL_GetKeyboardState(NULL); 
    if (keys[SDL_SCANCODE_W])self->position.y -= 1;
    if (keys[SDL_SCANCODE_A])self->position.x -= 1;
    if (keys[SDL_SCANCODE_S])self->position.y += 1;
    if (keys[SDL_SCANCODE_D])self->position.x += 1;
}

/*eol@eof*/

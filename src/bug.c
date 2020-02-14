#include "simple_logger.h"
#include "collisions.h"

#include "bug.h"
#include "level.h"

#define ES_DEAD 1

void bug_think(Entity *self)
{
    int mx,my;
    Uint32 buttons;
    if (!self)return;   
    self->age++;
    switch (self->state)
    {
        case ES_DEAD:
            self->frame = self->frame + 0.1;
            if (self->frame > 63)entity_free(self);
            break;
        default:
            self->frame = self->frame + 0.1;
            if (self->frame > 10)self->frame = 0;
    }
    buttons = SDL_GetMouseState(&mx,&my);
    if (buttons)
    {
        if (collide_circle(self->position, self->radius, vector2d(mx,my), 1))
        {
            self->state = ES_DEAD;
            self->frame = 59;
            vector2d_set(self->velocity,0,0);
            return;
        }
    }
}

void bug_touch(Entity *self,Entity *other)
{
    if ((!self) || (!other))return;
    if ((!self->madebabies) && (self->age > 100))
    {    
        bug_new(vector2d(self->position.x + gfc_crandom() * 64,self->position.y + gfc_crandom() * 64),vector2d(gfc_crandom(),gfc_crandom()));
        self->madebabies = 1;
    }
}

Entity *bug_new(Vector2D position,Vector2D velocity)
{
    Entity *self;
    self = entity_new();
    if (!self)return NULL;
    self->sprite = gf2d_sprite_load_all(
        "images/space_bug_top.png",
        128,
        128,
        16);
    self->radius = 24;
    self->size.x = 32;
    self->size.y = 32;
    self->think = bug_think;
    self->touch = bug_touch;
    vector2d_copy(self->position,position);
    vector2d_copy(self->velocity,velocity);
    vector2d_set(self->drawOffset,-64,-78);
    return self;
}


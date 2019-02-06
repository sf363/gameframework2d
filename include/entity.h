#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "gf2d_sprite.h"

typedef enum
{
    ES_Idle,
    ES_MovingToOregon,
    ES_Pain,
    ES_Attacking,
    ES_MAX
}EntityState;

typedef struct Entity_S
{
    int _inuse;
    Vector2D position;
    Vector2D velocity;
    Vector2D scale;
    Sprite *sprite;
    float frame;
    void (*update)(struct Entity_S * self);
    struct Entity_S * target;
}Entity;

/**
 * @brief initialize internal entity manager system
 * @param maxEntities maximum total entities to support
 */
void entity_system_init(Uint32 maxEntities);

/**
 * @brief return a pointer to a new entity
 * @returns NULL if out of entities or system not initialized, a pointer to a blank entity otherwise
 */
Entity *entity_new();

/**
 * @brief draw an entity
 * @param self the entity to draw
 */
void entity_draw(Entity *self);

/**
 * @brief draw all active entities
 */
void entity_draw_all();

/**
 * @brief update an entity
 * @param ent the entity to update
 */
void entity_update(Entity *ent);

/**
 * @brief update all active entities
 */
void entity_update_all();

/**
 * @brief free an entity
 * @param self the entity to free
 */
void entity_free(Entity *self);

#endif

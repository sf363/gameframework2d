#include <stdlib.h>
#include <string.h>
#include "entity.h"
#include "simple_logger.h"
#include "gf2d_sprite.h"

typedef struct
{
    Entity *entityList;
    Uint32  maxEntities;
}EntityManager;

static EntityManager entityManager = {0};

void entity_system_close()
{
    int i;
    for (i = 0; i < entityManager.maxEntities; i++)
    {
        entity_free(&entityManager.entityList[i]);
    }
    if (entityManager.entityList)
    {
        free(entityManager.entityList);
    }
    entityManager.maxEntities = 0;
    entityManager.entityList = NULL;
}

void entity_system_init(Uint32 maxEntities)
{
    if (!maxEntities)
    {
        slog("cannot allocate zero entities");
        return;
    }
    entityManager.entityList = (Entity*)malloc(sizeof(Entity)*maxEntities);
    if (!entityManager.entityList)
    {
        slog("failed to allocate %i entities for system",maxEntities);
    }
    entityManager.maxEntities = maxEntities;
    memset(entityManager.entityList, 0, sizeof(Entity)*maxEntities);
    atexit(entity_system_close);
}

Entity *entity_new()
{
    int i;
    for (i = 0; i < entityManager.maxEntities; i++)
    {
        if (entityManager.entityList[i]._inuse)continue;
        entityManager.entityList[i]._inuse = 1;
        entityManager.entityList[i].scale.x = 1;
        entityManager.entityList[i].scale.y = 1;
        return &entityManager.entityList[i];
    }
    slog("all entities in use, cannot provide a new entity");
    return NULL;
}

void entity_draw(Entity *self)
{
    if (!self)return;
    if (!self->_inuse)return;
    gf2d_sprite_draw(
        self->sprite,
        self->position,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        (int)self->frame);
    
}

void entity_draw_all()
{
    int i;
    for (i = 0; i < entityManager.maxEntities; i++)
    {
        entity_draw(&entityManager.entityList[i]);
    }
}

void entity_update(Entity *ent)
{
    if ((!ent)||(!ent->_inuse))return;
    ent->frame += 0.1;
    if (ent->frame >= 16)ent->frame = 0;
    vector2d_add(ent->position,ent->position,ent->velocity);
    if (ent->update)ent->update(ent);
}

void entity_update_all()
{
    int i;
    for (i = 0; i < entityManager.maxEntities; i++)
    {
        entity_update(&entityManager.entityList[i]);
    }
}



void entity_free(Entity *self)
{
    if (!self)return;
    if (self->sprite != NULL)
    {
        gf2d_sprite_free(self->sprite);
    }
    memset(self,0,sizeof(Entity));
}


/*eol@eof*/

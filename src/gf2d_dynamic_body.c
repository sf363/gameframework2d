#include "gf2d_dynamic_body.h"
#include "gf2d_collision.h"
#include "simple_logger.h"
#include <stdlib.h>

Shape gf2d_dynamic_body_to_shape(DynamicBody *a)
{
    Shape aS = {0};
    if (!a)return aS;
    gf2d_shape_copy(&aS,*a->body->shape);
    gf2d_shape_move(&aS,a->position);
    return aS;
}

Uint8 gf2d_dynamic_body_collision_check(DynamicBody *dba,DynamicBody *dbb)
{
    if ((!dba)||(!dbb))return 0;
    if ((!dba->body)||(!dbb->body))
    {
        slog("cannot do collision check, body missing from one or more DynamicBody");
        return 0;
    }
    return gf2d_shape_overlap(gf2d_dynamic_body_to_shape(dba),gf2d_dynamic_body_to_shape(dbb));
}

DynamicBody *gf2d_dynamic_body_new()
{
    DynamicBody *db;
    db = (DynamicBody *)malloc(sizeof(DynamicBody));
    if (!db)
    {
        slog("failed to allocation data for a new dynamic body");
        return NULL;
    }
    memset(db,0,sizeof(DynamicBody));
    return db;
}

void gf2d_dynamic_body_clear_collisions(DynamicBody *db)
{
    int i, count;
    Collision *collision;
    if (!db)return;
    count = gf2d_list_get_count(db->collisionList);
    for (i = 0; i < count;i++)
    {
        collision = (Collision*)gf2d_list_get_nth(db->collisionList,i);
        if (!collision)continue;
        gf2d_collision_free(collision);
    }
}

void gf2d_dynamic_body_free(DynamicBody *db)
{
    if (!db)return;
    //cleanup collionList
    gf2d_dynamic_body_clear_collisions(db);
    free(db);
}

void gf2d_dynamic_body_update(DynamicBody *db)
{
    if (!db)return;
    if (!db->body)return;
    vector2d_copy(db->body->position,db->position);
}

void gf2d_dynamic_body_reset(DynamicBody *db,float factor)
{
    if (!db)return;
    gf2d_dynamic_body_clear_collisions(db);
    vector2d_copy(db->position,db->body->position);
    vector2d_scale(db->velocity,db->body->velocity,factor);
}


/*eol@eof*/

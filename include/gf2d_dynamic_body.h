#ifndef __GF2D_DYNAMIC_BODY_H__
#define __GF2D_DYNAMIC_BODY_H__

#include "gf2d_body.h"

typedef struct
{
    Body       *body;
    Uint8       blocked;        /**<true once this body has been blocked and no longer moves*/
    Vector2D    position;       /**<temp position during update*/
    Vector2D    velocity;      /**<scaled velocity based on space step*/
    List       *collisionList;  /**<list of collisions accrued during space update*/
}DynamicBody;

DynamicBody *gf2d_dynamic_body_new();

void gf2d_dynamic_body_free(DynamicBody *db);

/**
 * @brief reset a dynamic body before a space update
 * @param db the dynamic body to reset
 * @param factor the time set factor to update by
 */
void gf2d_dynamic_body_reset(DynamicBody *db,float factor);

/**
 * @brief after a space update, this writes back to the body the new data and calls update functions
 * @param db the dynamic body that has been updated
 */
void gf2d_dynamic_body_update(DynamicBody *db);

/**
 * @brief check if two dynamic bodies are overlapping at the current time step
 * @param dba the first dynamic body, the one moving
 * @param dbb the other dynamic body, the one not moving
 * @return 0 on no collision or error, 1 if there is a collision
 */
Uint8 gf2d_dynamic_body_collision_check(DynamicBody *dba,DynamicBody *dbb);

#endif

#ifndef __GF2D_DYNAMIC_BODY_H__
#define __GF2D_DYNAMIC_BODY_H__

#include "gf2d_body.h"
#include "gf2d_collision.h"

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
 * @note the collision object returned must be freed
 * @param dba the first dynamic body, the one moving
 * @param dbb the other dynamic body, the one not moving
 * @param timeStep the time segment for the current update round
 * @return NULL on error or no collision, a pointer to the collision data otherwise
 */
Collision *gf2d_dynamic_body_collision_check(DynamicBody *dba,DynamicBody *dbb,float timeStep);

/**
 * @brief check if a dynamic body is clipping an arbitrary shape
 * @note the collision object returned must be freed
 * @param dba the first dynamic body, the one moving
 * @param shape the arbitrary shape to check against
 * @param timeStep the time segment for the current update round
 * @return NULL on error or no collision, a pointer to the collision data otherwise
 */
Collision *gf2d_dynamic_body_shape_collision_check(DynamicBody *dba,Shape *shape,float timeStep);

/**
 * @brief check if a dynamic body is clipping the bounds of a space
 * @note the collision object returned must be freed
 * @param dba the first dynamic body, the one moving
 * @param bounds the outer boundary of the physics space
 * @param timeStep the time segment for the current update round
 * @return NULL on error or no collision, a pointer to the collision data otherwise
 */
Collision *gf2d_dynamic_body_bounds_collision_check(DynamicBody *dba,Rect bounds,float timeStep);


#endif

#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "entity.h"

/**
* @brief spawn a player entity
* @param position the screen position to spawn the player at
* @return NULL on error, or a pointer to a new player entity
*/
Entity *player_spawn(Vector2D position);
Entity *mage_spawn(Vector2D position);
Entity *healer_spawn(Vector2D position);
Entity *ranger_spawn(Vector2D position);



#endif
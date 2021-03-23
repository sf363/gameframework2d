#include "door.h"
#include "player.h"
#include "level.h"
#include "simple_logger.h"
#include "entity.h"


void door_draw(Entity *self);
void door_think(Entity *self);
void door_update(Entity *self);
void door_activate(Entity *self, Entity *activator);
int  door_touch(Entity *self, Entity *other);
void door_die(Entity *self);
Entity *door_new(Vector2D position, const char *nextLevel, const char *target, Uint32 id);

Entity *door_spawn(Vector2D position)//1
{
	Entity *ent;
	ent = entity_new();
	if (!ent)
	{
		slog("failed to create entity for the player");
		return NULL;
	}
	ent->think = door_think;
	ent->sprite = gf2d_sprite_load_all("images/castledoors.png", 200,200, 16);
	vector2d_copy(ent->position, position);
	ent->frameRate = 0.1;
	ent->frameCount = 1;
	ent->update = door_update;
	ent->rotation.x = 64;
	ent->rotation.y = 64;
	return ent;
}
void door_update(Entity *self)
{
	
}
void door_think(Entity *self)
{
	
}

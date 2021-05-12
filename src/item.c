#include "simple_logger.h"

#include "player.h"
#include "item.h"
#include "gfc_audio.h"

void item_think(Entity *self);
void item_update(Entity *self);


Entity *item_spawn(Vector2D position)
{
	Entity *ent;
	ent = entity_new();
	if (!ent)
	{
		slog("failed to create entity for the player");
		return NULL;
	}
	Sound *pickup = gf2d_sound_load("sounds/coin.mp3", 0.5, 1);
	ent->Audio = pickup;
	ent->radius = 100;
	ent->health = 50;
	ent->think = item_think;
	ent->sprite = gf2d_sprite_load_all("images/keyicons.png", 75, 80, 1);
	vector2d_copy(ent->position, position);
	ent->frameRate = 0.1;
	ent->frameCount = 1;
	ent->update = item_update;
	ent->rotation.x = 64;
	ent->rotation.y = 64;
	ent->turn = 0;
	ent->attack = 100;
	ent->complete = 1;
	ent->name = 4; // 4 = keys
	return ent;
}

void item_think(Entity *self) //collecting keys for player //fix
{
	if (self->target != NULL)
	{
		if (self->turn == 1)
		{
			self->turn = 0;
			self->complete = 1;
			self->target->turn = 1;
			self->target->complete = 0;
			if (self->health <= 0)
			{
				slog("captured key died");
				self->dead = 1;
				gf2d_sound_play(self->Audio, 0, 1, 2, -1);
				self->target->keys++;
				//entity_free(self);
			}
		}
		if (self->dead == 1)
		{
			self->target->XP += 25;
			entity_free(self);
			//self->target->target = monster_spawn, imp_spawn, zombie_spawn, skeleton_spawn, soul_reaper_spawn;

		}
	}
}

void item_update(Entity *self)
{

}


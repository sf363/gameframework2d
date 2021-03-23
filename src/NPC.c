#include "simple_logger.h"

#include "player.h"
#include "NPC.h"

void NPC_think(Entity *self);
void NPC_update(Entity *self);
void shop(Entity *self, int amount, Entity *other);

Entity *NPC_spawn(Vector2D position)
{
	Entity *ent;
	ent = entity_new();
	if (!ent)
	{
		slog("failed to create entity for the player");
		return NULL;
	}
	ent->radius = 100;
	ent->think = NPC_think;
	ent->sprite = gf2d_sprite_load_all("images/npc-oldman1.png",23,32,1);
	vector2d_copy(ent->position, position);
	ent->frameRate = 0.1;
	ent->frameCount = 1;
	ent->update = NPC_update;
	ent->rotation.x = 64;
	ent->rotation.y = 64;
	ent->turn = 1;
	ent->attack = 100;
	ent->complete = 0;
	ent->name = 3; // 3 = NPCs
	return ent;
}

void NPC_think(Entity *self)
{
	const Uint8 *keys;
	keys = SDL_GetKeyboardState(NULL);
	if (self->start == 1)
	{

		if (self->target != NULL)
		{
			if (self->turn == 1)
			{

				if (keys[SDL_SCANCODE_UP])
				{
					self->target->health += self->attack;
					self->turn = 0;
					self->complete = 1;
					self->target->turn = 1;
					self->target->complete = 0;
					slog("The NPC healed you");
				}
			}

		}
	}

}
void NPC_update(Entity *self)
{
	
}


void shop(Entity *self, int amount, Entity *other)
{

}
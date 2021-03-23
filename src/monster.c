#include "simple_logger.h"

#include "player.h"
#include "monster.h"
#include "entity.h"

void monster_think(Entity *self);
void imp_think(Entity *self);
void zombie_think(Entity *self);
void skel_think(Entity *self);
void soul_think(Entity *self);
void monster_update(Entity *self);
int  monster_damage(Entity *self, int amount, Entity *source);

void monster_die(Entity *self);
static Entity *_monster = NULL;
Entity *monster_spawn(Vector2D position)//1
{
	Entity *ent;
	ent = entity_new();
	if (!ent)
	{
		slog("failed to create entity for the player");
		return NULL;
	}
	ent->think = monster_think;
	ent->damage = monster_damage;
	//ent->die = monster_die;
	ent->radius = 50;
	ent->health = 75;
	ent->attack = 10;
	ent->sprite = gf2d_sprite_load_all("images/druid2.png", 128, 128, 16);
	vector2d_copy(ent->position, position);
	ent->frameRate = 0.1;
	ent->frameCount = 1;
	ent->update = monster_update;
	ent->rotation.x = 64;
	ent->rotation.y = 64;
	ent->turn = 0;
	ent->complete = 1;
	ent->dead = 0;
	ent->name = 2;
	return ent;
}
Entity *imp_spawn(Vector2D position)//2
{
	Entity *ent;
	ent = entity_new();
	if (!ent)
	{
		slog("failed to create entity for the player");
		return NULL;
	}
	ent->think = imp_think;
	ent->damage = monster_damage;
	//ent->die = monster_die;
	ent->health = 50;
	ent->radius = 50;
	ent->attack = 5;
	ent->sprite = gf2d_sprite_load_all("images/imp2.png", 128, 128, 16);
	vector2d_copy(ent->position, position);
	ent->frameRate = 0.1;
	ent->frameCount = 1;
	ent->update = monster_update;
	ent->rotation.x = 64;
	ent->rotation.y = 64;
	ent->turn = 0;
	ent->complete = 1;
	ent->dead = 0;
	ent->name = 2;
	return ent;
}
Entity *soul_reaper_spawn(Vector2D position)//3
{
	Entity *ent;
	ent = entity_new();
	if (!ent)
	{
		slog("failed to create entity for the player");
		return NULL;
	}
	ent->think = soul_think;
	ent->damage = monster_damage;
	//ent->die = monster_die;
	ent->health = 200;
	ent->radius = 50;
	ent->attack = 20;
	ent->sprite = gf2d_sprite_load_all("images/shadow_soul2.png", 128, 128, 16);
	vector2d_copy(ent->position, position);
	ent->frameRate = 0.1;
	ent->frameCount = 1;
	ent->update = monster_update;
	ent->rotation.x = 64;
	ent->rotation.y = 64;
	ent->turn = 0;
	ent->complete = 1;
	ent->dead = 0;
	ent->name = 2;
	return ent;
}
Entity *skeleton_spawn(Vector2D position)//4
{
	Entity *ent;
	ent = entity_new();
	if (!ent)
	{
		slog("failed to create entity for the player");
		return NULL;
	}
	ent->think = skel_think;
	ent->damage = monster_damage;
	//ent->die = monster_die;
	ent->health = 75;
	ent->radius = 50;
	ent->attack = 13;
	ent->sprite = gf2d_sprite_load_all("images/skeleton3.png", 128, 128, 16);
	vector2d_copy(ent->position, position);
	ent->frameRate = 0.1;
	ent->frameCount = 1;
	ent->update = monster_update;
	ent->rotation.x = 64;
	ent->rotation.y = 64;
	ent->turn = 0;
	ent->complete = 1;
	ent->dead = 0;
	ent->name = 2;
	return ent;
}
Entity *zombie_spawn(Vector2D position)//5
{
	Entity *ent;
	ent = entity_new();
	if (!ent)
	{
		slog("failed to create entity for the player");
		return NULL;
	}
	ent->think = zombie_think;
	ent->damage = monster_damage;
	//ent->die = monster_die;
	ent->health = 50;
	ent->radius = 50;
	ent->attack = 11;
	ent->sprite = gf2d_sprite_load_all("images/zombie2.png", 128, 128, 16);
	vector2d_copy(ent->position, position);
	ent->frameRate = 0.1;
	ent->frameCount = 1;
	ent->update = monster_update;
	ent->rotation.x = 64;
	ent->rotation.y = 64;
	ent->turn = 0;
	ent->complete = 1;
	ent->dead = 0;
	ent->name = 2;
	return ent;
}
void monster_update(Entity *self)
{
	/*Vector2D aimdir;
	float angle;
	int mx, my;
	if (!self)return;
	SDL_GetMouseState(&mx, &my);
	aimdir.x = mx - (self->position.x + 64);
	aimdir.y = my - (self->position.y + 64);
	angle = vector_angle(aimdir.x, aimdir.y);
	self->rotation.z = angle + 90;*/
}
void monster_think(Entity *self)
{
	if (self->target != NULL)
	{
		if (self->turn == 1)
		{
			self->target->health -= self->attack;
			self->turn = 0;
			self->complete = 1;
			self->target->turn = 1;
			self->target->complete = 0;
			slog("monster attacked you");
			if (self->health <= 0)
			{
				slog("monster died");
				self->dead = 1;
				//entity_free(self);
			}
		}
		if (self->dead == 1)
		{
			//self->target->turn = 1;
			//self->target->complete = 0;
			//self->target->start = 0;
			//self->start = 0;
			self->target->XP += 100;
			entity_free(self);
			self->dead = 0;
			self->target->target = monster_spawn, imp_spawn, zombie_spawn, skeleton_spawn, soul_reaper_spawn;
		}
	}
	
}
void imp_think(Entity *self)
{
	if (self->target != NULL)
	{
		if (self->turn == 1)
		{
			self->target->health -= self->attack;
			self->turn = 0;
			self->complete = 1;
			self->target->turn = 1;
			self->target->complete = 0;
			slog("imp attacked you");

			if (self->health <= 0)
			{
				slog("monster died");
				self->dead = 1;
				self->turn = 0;
				self->complete = 1;
				self->target->turn = 1;
				self->target->complete = 0;
				//entity_free(self);
			}
		}
		if (self->dead == 1)
		{
			self->target->XP += 100;
			entity_free(self);
			self->target->target = NULL;
			self->start = 0;
			self->target->start = 0;
		
		}
	}

}
void zombie_think(Entity *self)
{
	if (self->target != NULL)
	{
		if (self->turn == 1)
		{
			self->target->health -= self->attack;
			self->turn = 0;
			self->complete = 1;
			self->target->turn = 1;
			self->target->complete = 0;
			slog("zombie attacked you");
			if (self->health <= 0)
			{
				slog("monster died");
				self->dead = 1;
				//entity_free(self);
			}
		}
		if (self->dead == 1)
		{
			self->target->XP += 100;
			entity_free(self);
			self->target->target = monster_spawn, imp_spawn, zombie_spawn, skeleton_spawn, soul_reaper_spawn;
		
		}
	}

}
void skel_think(Entity *self)
{
	if (self->target != NULL)
	{
		if (self->turn == 1)
		{
			self->target->health -= self->attack;
			self->turn = 0;
			self->complete = 1;
			self->target->turn = 1;
			self->target->complete = 0;
			slog("skel attacked you");
			if (self->health <= 0)
			{
				slog("monster died");
				self->dead = 1;
				//entity_free(self);
			}
		}
		if (self->dead == 1)
		{
			self->target->XP += 100;
			entity_free(self);
			self->target->target = monster_spawn, imp_spawn, zombie_spawn, skeleton_spawn, soul_reaper_spawn;
			
		}
	}

}
void soul_think(Entity *self)
{
	if (self->target != NULL)
	{
		if (self->turn == 1)
		{
			self->target->health -= self->attack;
			self->turn = 0;
			self->complete = 1;
			self->target->turn = 1;
			self->target->complete = 0;
			slog("soul reaper attacked you");
			if (self->health <= 0)
			{
				slog("monster died");
				self->dead = 1;
				//entity_free(self);
			}
		}
		if (self->dead == 1)
		{
			self->target->XP += 100;
			entity_free(self);
			self->target->target = monster_spawn, imp_spawn, zombie_spawn, skeleton_spawn, soul_reaper_spawn;
	
		}
	}

}
int monster_damage(Entity *self, int amount, Entity *source)
{
	/*int taken;
	taken = MIN(self->health, amount);
	self->health -= amount;
	slog("monster taking %i damage", amount);
	slog("monster health %f", self->health);
	if (self->health <= 0)
	{
		self->die(self);
		slog("Monster has died");
		//self->dead = 1;
		return taken;
	}
	return taken;*/
}

void monster_die(Entity *self)
{
	if (self->target != NULL)
	{
		//this is where we give the teams xp
		if (self->dead == 1)
		{
			slog("monster's gone");
			entity_free(self);
			self->XP += 100;
			self->dead = 0;
		}
	}
}
void die(Entity *self)
{
	entity_free(_monster);
	self-> dead = 0;
}
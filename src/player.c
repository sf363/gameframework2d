#include "simple_logger.h"

#include "player.h"
#include "camera.h"


void player_update(Entity *self);
void player_think(Entity *self);
void team_think(Entity *self);
void team_update(Entity *self);
void mage_think(Entity *self);
void tank_update(Entity *self);
void ranger_update(Entity *self);
int player_health(Entity *self);
int  player_damage(Entity *self, int amount, Entity *source);
void player_die(Entity *self);
void levelup(Entity *self);
static Entity *_player = NULL;
int p_health;
Entity *player_spawn(Vector2D position)
{
	Entity *ent;
	ent = entity_new();
	if (!ent)
	{
		slog("failed to create entity for the player");
		return NULL;
	}
	ent->sprite = gf2d_sprite_load_all("images/ed210_top.png", 128, 128, 16);
	ent->health = 100;
	ent->attack = 12;
	ent->damage = player_damage;
	vector2d_copy(ent->position, position);
	ent->radius = 100;
	ent->attack2 = 25;
	ent->attack3 = 35;
	ent->frameRate = 0.1;
	ent->frameCount = 16;
	ent->update = player_update;
	ent->get = player_health;
	ent->think = player_think;
	ent->rotation.x = 64;
	ent->rotation.y = 64;
	ent->turn = 1;
	ent->complete = 0;
	ent->lvl = 1;
	ent->XP=0;
	ent->name = 1;
	return ent;
}
void player_update(Entity *self)
{
	Vector2D camera;
	Vector2D cameraSize;

	if (!self)return;
	cameraSize = camera_get_dimensions();
	camera.x = (self->position.x + 64) - (cameraSize.x * 0.5);
	camera.y = (self->position.y + 64) - (cameraSize.y * 0.5);
	camera_set_position(camera);
	// apply dampening on velocity
	vector2d_scale(self->velocity, self->velocity, 0.75);
	if (vector2d_magnitude_squared(self->velocity) < 2)
	{
		vector2d_clear(self->velocity);
	}
}
void team_think(Entity *self)
{
	const Uint8 *keys;
	Vector2D aimdir, camera, thrust;
	float angle;
	int mx, my;
	if (!self)return;
	keys = SDL_GetKeyboardState(NULL);
	SDL_GetMouseState(&mx, &my);
	camera = camera_get_position();
	mx += camera.x;
	my += camera.y;
	aimdir.x = mx - (self->position.x + 64);
	aimdir.y = my - (self->position.y + 64);
	angle = vector_angle(aimdir.x, aimdir.y);
	self->rotation.z = angle + 90;

	// turn aimdir into a unit vector
	vector2d_normalize(&aimdir);
	// check for motion
	if (keys[SDL_SCANCODE_W])
	{
		vector2d_scale(thrust, aimdir, 5);
		vector2d_add(self->velocity, self->velocity, thrust);
	}
	vector2d_scale(self->velocity, self->velocity, 0.75);
	if (vector2d_magnitude_squared(self->velocity) < 2)
	{
		vector2d_clear(self->velocity);
	}
}
void team_update(Entity *self)
{

}
void player_think(Entity *self)
{
	int player_position;
	const Uint8 *keys;
	Vector2D aimdir, camera, thrust;
	float angle;
	int mx, my;
	if (!self)return;
	keys = SDL_GetKeyboardState(NULL);
	SDL_GetMouseState(&mx, &my);
	camera = camera_get_position();
	mx += camera.x;
	my += camera.y;
	aimdir.x = mx - (self->position.x + 64);
	aimdir.y = my - (self->position.y + 64);
	angle = vector_angle(aimdir.x, aimdir.y);
	self->rotation.z = angle + 90;
	
	// turn aimdir into a unit vector
	vector2d_normalize(&aimdir);
	// check for motion
	if (keys[SDL_SCANCODE_W])
	{
		vector2d_scale(thrust, aimdir, 5);
		vector2d_add(self->velocity, self->velocity, thrust);
	}
	if (self->start == 1)
	{

		if (self->target != NULL)
		{
			if (self->target->name == 2)// interaction with monster
			{
				if (self->turn == 1)
				{

					if (keys[SDL_SCANCODE_UP])//attack1
					{
						p_health = self->health;
						self->target->health -= self->attack;
						self->turn = 0;
						self->complete = 1;
						self->target->turn = 1;
						self->target->complete = 0;
						slog("you useed attack1 the monster");
						//slog("youre health is " + p_health);
					}
					if (keys[SDL_SCANCODE_LEFT])//attack2
					{
						p_health = self->health;
						self->target->health -= self->attack2;
						self->turn = 0;
						self->complete = 1;
						self->target->turn = 1;
						self->target->complete = 0;
						slog("you useed attack2 the monster");
						//slog("youre health is " + p_health);
					}
					if (keys[SDL_SCANCODE_RIGHT])//attack 3
					{
						p_health = self->health;
						self->target->health -= self->attack3;
						self->turn = 0;
						self->complete = 1;
						self->target->turn = 1;
						self->target->complete = 0;
						slog("you useed attack3 the monster");
						//slog("youre health is " + p_health);
					}
				}
			}
			if (self->target->name == 3) //interaction with NPC
			{
				if (self->turn == 1)
				{

					if (keys[SDL_SCANCODE_UP])
					{
						self->turn = 0;
						self->complete = 1;
						self->target->turn = 1;
						self->target->complete = 0;
						slog("Player got healed");
					}
				}
			}
			if (self->XP == 100)
			{
				self->lvl++;
				slog("you leveled up");
				self->XP = 0;
			}
			if (self->target->dead == 1)
			{
				//switch target to another monster
				self->turn = 1;
				self->complete = 0;
				self->target->turn = 0;
				self->target->complete = 1;
			}
		}
	}


}
Entity *mage_spawn(Vector2D position)
{
	Entity *ent;
	ent = entity_new();
	if (!ent)
	{
		slog("failed to create entity for the player");
		return NULL;
	}
	//ent->target->health -= 10;
	ent->health = 100;
	ent->mana = 100;
	ent->damage = player_damage;
	ent->die = player_die;
	ent->attack = 2;
	ent->attack2 = 30;
	ent->attack3 = 40;
	ent->radius = 100;
	ent->sprite = gf2d_sprite_load_all("images/ed210_top.png", 128, 128, 16);
	vector2d_copy(ent->position, position);
	ent->frameRate = 0.1;
	ent->frameCount = 16;
	ent->think = team_think;
	ent->update = team_update;
	ent->rotation.x = 64;
	ent->rotation.y = 64;
	ent->turn = 0;
	ent->complete = 1;
	return ent;
}
/*void mage_think(Entity *self)
{
	const Uint8 *keys;
	keys = SDL_GetKeyboardState(NULL);
	if (self->target != NULL)
	{
		if (self->turn == 1)
		{

			if (keys[SDL_SCANCODE_UP])
			{
				self->target->health -= self->attack;
				self->turn = 0;
				self->complete = 1;
				self->target->turn = 1;
				self->target->complete = 0;
				slog("you attacked the monster");
			}
		}

	}
}*/

Entity *tank_spawn(Vector2D position) //tank player
{
	Entity *ent;
	ent = entity_new();
	if (!ent)
	{
		slog("failed to create entity for the player");
		return NULL;
	}
	ent->health = 200;
	ent->mana = 50;
	ent->damage = player_damage;
	ent->die = player_die;
	ent->attack = 5;
	ent->attack2 = 10;
	ent->attack3 = 12;
	ent->radius = 100;
	ent->sprite = gf2d_sprite_load_all("images/ed210_top.png", 128, 128, 16);
	vector2d_copy(ent->position, position);
	ent->frameRate = 0.1;
	ent->frameCount = 16;
	ent->think = team_think;
	ent->update = team_update;
	ent->rotation.x = 64;
	ent->rotation.y = 64;
	ent->turn = 0;
	ent->complete = 1;
	return ent;
}

Entity *ranger_spawn(Vector2D position) //ranger players
{
	Entity *ent;
	ent = entity_new();
	if (!ent)
	{
		slog("failed to create entity for the player");
		return NULL;
	}
	ent->health = 100;
	ent->mana = 0;
	ent->attack = 12;
	ent->attack2 = 20;
	ent->attack3 = 26;
	ent->radius = 100;
	ent->damage = player_damage;
	ent->die = player_die;
	ent->sprite = gf2d_sprite_load_all("images/ed210_top.png", 128, 128, 16);
	vector2d_copy(ent->position, position);
	ent->frameRate = 0.1;
	ent->frameCount = 16;
	ent->think = team_think;
	ent->update = team_update;
	ent->rotation.x = 64;
	ent->rotation.y = 64;
	ent->turn = 0;
	ent->complete = 1;
	return ent;
}

/*
void mage_update(Entity *self)
{
	Vector2D aimdir;
	float angle;
	int mx, my;
	if (!self)return;
	SDL_GetMouseState(&mx, &my);
	aimdir.x = mx - (self->position.x + 64);
	aimdir.y = my - (self->position.y + 64);
	angle = vector_angle(aimdir.x, aimdir.y);
	self->rotation.z = angle + 90;
}
void healer_update(Entity *self)
{
	Vector2D aimdir;
	float angle;
	int mx, my;
	if (!self)return;
	SDL_GetMouseState(&mx, &my);
	aimdir.x = mx - (self->position.x + 64);
	aimdir.y = my - (self->position.y + 64);
	angle = vector_angle(aimdir.x, aimdir.y);
	self->rotation.z = angle + 90;
}
void ranger_update(Entity *self)
{
	Vector2D aimdir;
	float angle;
	int mx, my;
	if (!self)return;
	SDL_GetMouseState(&mx, &my);
	aimdir.x = mx - (self->position.x + 64);
	aimdir.y = my - (self->position.y + 64);
	angle = vector_angle(aimdir.x, aimdir.y);
	self->rotation.z = angle + 90;
}*/
// levels up the player if they have 50 xp

void levelup(Entity *self)
{
	if (self->XP = 100)
	{
		self->lvl++;
		self->XP = 0;
		slog("you leveled up");
	}
}
int player_damage(Entity *self, int amount, Entity *source)
{
	slog("player taking %i damage!", amount);
	self->health -= amount;
	if (self->health <= 0)self->health = 0;
	{
		self->die(self);
		slog("Player died");
	}
	
	return amount;
}
void player_die(Entity *self)
{
	if (self->health == 0)
	{
		slog("player died");
		entity_free(self);
	}
}
int player_health(Entity *self)
{
	return self->health;
}

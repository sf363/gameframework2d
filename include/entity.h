#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "gfc_types.h"
#include "gfc_vector.h"
#include "gf2d_sprite.h"

typedef struct Entity_s
{
	Bool       _inuse;
	Vector2D    position;
	Vector2D    get_position;
	Vector2D    velocity;
	Vector3D    rotation; //(x,y) = rotation center, z = degrees of rotation
	Sprite     *sprite;
	float       frame;
	float       frameRate;
	int         frameCount;
	float       radius;
	void(*update)(struct Entity_s *self);
	void(*get)(struct Entity_s *self);
	void(*think)(struct Entity_s *self);
	void(*draw)(struct Entity_s *self);
	//void(*free)(struct Entity_s *self);
	int(*damage)(struct Entity_s *self, int amount, struct Entity_s *source);
	int(*abilities)(struct Entity_s *self, int damage_a ,int mana_a);
	void(*die)(struct Entity_s *self);
	void(*levelup)(struct Entity_s *self);
	void(*shop)(struct Entity_s *self, int amount, struct Entity_s *other);
	void(*touch)(struct Entity_s *self, struct Entity_s *other);
	void(*activate)(struct Entity_s *self, struct Entity_s *activator);
	int dead;
	int name; // 1 = player, 2=monster, 3 = NPCs
	void       *data;
	struct Entity_s *target;
	int attack;
	int attack2;
	int attack3;
	int start; // starts battle
	float health;                        
	float mana;
	int attackType;
	float XP;
	int turn; // knows if its the players turn
	int complete; // knows when the player is done
	TextLine    targetLevel;                
	TextLine    targetEntity;
	Uint32      targetId;
	int lvl;
}Entity;


float player_level();

/**
* @brief initialize internal entity management system
* @param max_entities how many concurrent entities to support
*/
void entity_manager_init(Uint32 max_entities);

/**
* @brief calls update function on all entities
*/
void entity_manager_update_entities();


/**
* @brief call draw on all entities
*/
void entity_manager_draw_entities();


/**
* @brief free all entities in the system and destroy entity manager
*/
void entity_manager_free();

/**
* @brief allocate an entity, initialize it to zero and return a pointer to it
* @return NULL on error (see logs) or a pointer to an initialized entity.
*/
Entity *entity_new();

int gf2d_entity_deal_damage(Entity *target, Entity *inflictor, Entity *attacker, int damage);

/**
* @brief frees provided entity
* @param ent the entity to free
*/
void entity_free(Entity *ent);

/**
* @brief draws an entity to the current render frame
* @param ent the entity to draw
*/
void entity_draw(Entity *ent);

void entity_manager_init(Uint32 maxEnts);

int gf2d_entity_deal_damage(Entity *target, Entity *inflictor, Entity *attacker, int damage, Vector2D kick);

int collision(Entity *self, Entity *ent);

#endif
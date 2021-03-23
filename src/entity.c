#include <stdlib.h>
#include "simple_logger.h"

#include "entity.h"
#include "camera.h"

typedef struct
{
	Entity *entity_list;
	Uint32  max_entities;
}EntityManager;

static EntityManager entity_manager = { 0 };



void entity_manager_init(Uint32 max_entities)
{
	if (max_entities == 0)
	{
		slog("cannot allocate 0 entities!");
		return;
	}
	if (entity_manager.entity_list != NULL)
	{
		entity_manager_free();
	}
	entity_manager.entity_list = (Entity *)gfc_allocate_array(sizeof (Entity), max_entities);
	if (entity_manager.entity_list == NULL)
	{
		slog("failed to allocate entity list!");
		return;
	}
	entity_manager.max_entities = max_entities;
	atexit(entity_manager_free);
	slog("entity system initialized");
}

void entity_manager_free()
{
	if (entity_manager.entity_list != NULL)
	{
		free(entity_manager.entity_list);
	}
	memset(&entity_manager, 0, sizeof(EntityManager));
	slog("entity system closed");
}

void entity_update(Entity *self)
{
	if (!self)return;
	// DO ANY GENERIC UPDATE CODE
	vector2d_add(self->position, self->position, self->velocity);
	self->frame += self->frameRate;
	if (self->frame >= self->frameCount)self->frame = 0;
	// IF THERE IS A CUSTOM UPDATE, DO THAT NOW
	if (self->update)self->update(self);
}

void entity_manager_update_entities()
{
	int i;
	int j;
	if (entity_manager.entity_list == NULL)
	{
		slog("entity system does not exist");
		return;
	}
	for (i = 0; i < entity_manager.max_entities; i++)
	{
		if (entity_manager.entity_list[i]._inuse == 0)continue;
		entity_update(&entity_manager.entity_list[i]);
		for (j = 0; j < entity_manager.max_entities; j++)
		{
			if (i == j)
			{
				continue;
			}
			if (collision(&entity_manager.entity_list[i], &entity_manager.entity_list[j])== 1)
			{
				entity_manager.entity_list[i].start = 1;
				entity_manager.entity_list[j].start = 1;
				if (entity_manager.entity_list[i].name == 1 && entity_manager.entity_list[j].name == 2)
				{
					entity_manager.entity_list[i].position.x = 1500;
					entity_manager.entity_list[i].position.y = 100;
					entity_manager.entity_list[j].position.x = 1800;
					entity_manager.entity_list[j].position.y = 100;
					entity_manager.entity_list[i].target = &entity_manager.entity_list[j];
					entity_manager.entity_list[j].target = &entity_manager.entity_list[i];

				}
				
				//entity_manager.entity_list[i].target = &entity_manager.entity_list[j];
				//entity_manager.entity_list[j].target = &entity_manager.entity_list[i];
			
				//slog("collision");
				
				
			}
			
		}
	}
}
void entity_manager_think_entities()
{
	int i;
	for (i = 0; i < entity_manager.max_entities; i++)
	{
		if (entity_manager.entity_list[i]._inuse == 0)continue;
		if (entity_manager.entity_list[i].think != NULL)
		{
			entity_manager.entity_list[i].think(&entity_manager.entity_list[i]);
		}
	}
}

void entity_manager_draw_entities()
{
	int i;
	if (entity_manager.entity_list == NULL)
	{
		slog("entity system does not exist");
		return;
	}
	for (i = 0; i < entity_manager.max_entities; i++)
	{
		if (entity_manager.entity_list[i]._inuse == 0)continue;
		entity_draw(&entity_manager.entity_list[i]);
	}
}


Entity *entity_new()
{
	int i;
	if (entity_manager.entity_list == NULL)
	{
		slog("entity system does not exist");
		return NULL;
	}
	for (i = 0; i < entity_manager.max_entities; i++)
	{
		if (entity_manager.entity_list[i]._inuse)continue;// someone else is using this one
		memset(&entity_manager.entity_list[i], 0, sizeof(Entity));
		entity_manager.entity_list[i]._inuse = 1;
		return &entity_manager.entity_list[i];
	}
	slog("no free entities available");
	return NULL;
}

void entity_free(Entity *ent)
{
	if (!ent)
	{
		slog("cannot free a NULL entity");
		return;
	}
	gf2d_sprite_free(ent->sprite);
	ent->sprite = NULL;
	ent->_inuse = 0;
}

void entity_draw(Entity *ent)
{
	Vector2D drawPosition, offset;
	if (!ent)
	{
		slog("cannot draww a NULL entity");
		return;
	}
	if (ent->draw)
	{
		ent->draw(ent);
	}
	else
	{
		if (ent->sprite == NULL)
		{
			return;// nothing to draw
		}
		offset = camera_get_offset();
		if (!camera_rect_on_screen(gfc_sdl_rect(ent->position.x, ent->position.y, ent->sprite->frame_w, ent->sprite->frame_h)))
		{
			//entity is off camera, skip
			return;
		}
		drawPosition.x = ent->position.x + offset.x;
		drawPosition.y = ent->position.y + offset.y;

		gf2d_sprite_draw(
			ent->sprite,
			drawPosition,
			NULL,
			NULL,
			&ent->rotation,
			NULL,
			NULL,
			(Uint32)ent->frame);
	}
}
//this is for the turn base battle to start. If the player collides with the monster than the battle begins.
int collision(Entity *self, Entity *ent)
{
	float distance_x = self->position.x - ent->position.x;
	float distance_y = self->position.y - ent->position.y;
	if (self->radius == 0 || ent->radius == 0)
	{
		return 0;
	}
	float sum = self->radius + ent->radius;
	if ((distance_x*distance_x) + (distance_y*distance_y)<= sum * sum)
	{
		return 1;
	}
	return 0;

}

float player_level()
{
	
}

#include "collision.h"
#include "simple_logger.h"
#include "gf2d_draw.h"

void gf2d_body_clear(Body *body)
{
	if (!body)return;
	memset(body, 0, sizeof(Body));
}

void gf2d_body_set(
	Body       *body,
	char       *name,
	Uint32      layer,
	Uint32      team,
	Vector2D    position,
	Vector2D    velocity,
	float       mass,
	float       gravity,
	float       elasticity,
	Shape      *shape,
	void       *data,
	int(*bodyTouch)(struct Body_S *self, struct Body_S *other, Collision *collision),
	int(*worldTouch)(struct Body_S *self, Collision *collision))
{
	if (!body)return;
	body->layer = layer;
	body->team = team;
	vector2d_copy(body->position, position);
	vector2d_copy(body->velocity, velocity);
	body->mass = mass;
	body->gravity = gravity;
	body->elasticity = elasticity;
	body->shape = shape;
	body->data = data;
	body->bodyTouch = bodyTouch;
	body->worldTouch = worldTouch;
	//gf2d_word_cpy(body->name, name);
}

void gf2d_free_shapes(void *data,void *context)
{
    Shape *shape;
    if (!data)return;
    shape = (Shape*)data;
    free(shape);
}

void gf2d_space_free(Space *space)
{
    if (!space)return;
    gf2d_list_delete(space->bodyList);// bodies are not owned by the space and so will not be deleted by the space
    
    //static shapes ARE owned by the space, so are deleted when the space goes away
    gf2d_list_foreach(space->staticShapes,gf2d_free_shapes,NULL);
    gf2d_list_delete(space->staticShapes);
    free(space);
}

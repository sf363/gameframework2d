#ifndef __GF2D_COLLISION_H__
#define __GF2D_COLLISION_H__

#include "shape.h"
#include "gfc_list.h"
#include "gfc_text.h"

#define ALL_LAYERS 0xffffffff

typedef struct Collision_S Collision;

typedef struct Body_S
{
	TextLine    name;           /**<name for debugging purposes*/
	int         inactive;       /**<internal use only*/
	float       gravity;        /**<the factor this body adheres to gravity*/
	Uint32      layer;          /**<only bodies that share one or more layers will interact*/
	Uint32      team;           /**<bodies that share a team will NOT interact*/
	Vector2D    position;       /**<position of the center of mass*/
	Vector2D    velocity;       /**<rate of change of position over time*/
	Vector2D    newvelocity;    /**<after a collision this is the new calculated velocity*/
	float       mass;           /**<used for inertia*/
	float       elasticity;     /**<how much bounce this body has*/
	Shape      *shape;          /**<which shape data will be used to collide for this body*/
	void       *data;           /**<custom data pointer*/
	int(*bodyTouch)(struct Body_S *self, struct Body_S *other, Collision *collision);/**< function to call when two bodies collide*/
	int(*worldTouch)(struct Body_S *self, Collision *collision);/**<function to call when a body collides with a static shape*/
}Body;

struct Collision_S
{
	Uint8    collided;          /**<true if the there as a collision*/
	Vector2D pointOfContact;    /**<point in space that contact was made*/
	Vector2D normal;            /**<normal vector at the point of contact*/
	Shape   *shape;             /**<shape information on what what contacted*/
	Body    *body;              /**<body information if a body was collided with*/
	float    timeStep;          /**<at what time step contact was made*/
};

typedef struct
{
	List       *bodyList;       /**<list of bodies in the space*/
	List       *staticShapes;   /**<list of shapes that will collide that do not move*/
	int         precision;      /**<number of backoff attempts before giving up*/
	ShapeRect        bounds;         /**<absolute bounds of the space*/
	float       timeStep;       /**<how much each iteration of the simulation progresses time by*/
	Vector2D    gravity;        /**<global gravity pull direction*/
	float       dampening;      /**<rate of movement degrade  ambient frictions*/
	float       slop;           /**<how much to correct for body overlap*/
}Space;

void gf2d_body_clear(Body *body);

void gf2d_body_set(
	Body *body,
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
	int(*worldTouch)(struct Body_S *self, Collision *collision));

Space *gf2d_space_new();
Space *gf2d_space_new_full(
	int         precision,
	ShapeRect        bounds,
	float       timeStep,
	Vector2D    gravity,
	float       dampening,
	float       slop);
void gf2d_space_free(Space *space);
void gf2d_space_draw(Space *space);
void gf2d_space_add_body(Space *space, Body *body);
void gf2d_space_remove_body(Space *space, Body *body);
void gf2d_body_push(Body *body, Vector2D direction, float force);
void gf2d_space_add_static_shape(Space *space, Shape shape);
void gf2d_space_update(Space *space);
Collision gf2d_space_shape_test(Space *space, Shape shape);

#endif
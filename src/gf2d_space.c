#include "gf2d_space.h"
#include "gf2d_draw.h"
#include "simple_logger.h"

Uint8 gf2d_body_shape_collide(Body *a,Shape *s,Vector2D *poc, Vector2D *normal);

void gf2d_free_shapes(void *data,void *context)
{
    Shape *shape;
    if (!data)return;
    shape = (Shape*)data;
    free(shape);
}

void gf2d_space_body_collision_test(Space *space,Shape shape, Collision *collision)
{
    ClipFilter filter = {0};
    gf2d_space_body_collision_test_filter(space,shape, collision,filter);
}

void gf2d_space_shape_collision_test(Space *space,Shape shape, Collision *collision)
{
    int i,count;
    Shape *s;
    if ((!space)||(!collision))return;
    count = gf2d_list_get_count(space->staticShapes);
    for (i = 0; i < count; i++)
    {
        s = (Shape*)gf2d_list_get_nth(space->staticShapes,i);
        if (!s)continue;
        if(gf2d_shape_overlap_poc(shape, *s,&collision->pointOfContact, &collision->normal))
        {
            collision->collided = 1;
            collision->body = NULL;
            collision->shape = s;
            return;
        }
    }
}

Collision gf2d_space_shape_test_filter(Space *space,Shape shape, ClipFilter filter)
{
    Collision c;
    c.collided = 0;
    if (!space)return c;
    memset(&c,0,sizeof(Collision));
    gf2d_space_body_collision_test(space,shape, &c);
    if (c.collided)return c;
    if (filter.layer & WORLD_LAYER)
    {
        gf2d_space_shape_collision_test(space,shape, &c);
    }
    return c;
}

Collision gf2d_space_shape_test(Space *space,Shape shape)
{
    ClipFilter filter = {0};
    filter.layer = WORLD_LAYER;
    return gf2d_space_shape_test_filter(space,shape, filter);
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

Space *gf2d_space_new_full(
    int         precision,
    Rect        bounds,
    float       timeStep,
    Vector2D    gravity,
    float       dampening,
    float       slop)
{
    Space *space;
    space = gf2d_space_new();
    if (!space)return NULL;
    gf2d_rect_copy(space->bounds,bounds);
    vector2d_copy(space->gravity,gravity);
    space->timeStep = timeStep;
    space->precision = precision;
    space->dampening = dampening;
    space->slop = slop;
    return space;
}

Space *gf2d_space_new()
{
    Space *space;
    space = (Space *)malloc(sizeof(Space));
    if (!space)
    {
        slog("failed to allocate space for Space");
        return NULL;
    }
    memset(space,0,sizeof(Space));
    space->bodyList = gf2d_list_new();
    space->staticShapes = gf2d_list_new();
    return space;
}

void gf2d_space_add_static_shape(Space *space,Shape shape)
{
    Shape *newShape;
    if (!space)
    {
        slog("no space provided");
        return;
    }
    newShape = (Shape*)malloc(sizeof(shape));
    if (!newShape)
    {
        slog("failed to allocate new space for the shape");
        return;
    }
    memcpy(newShape,&shape,sizeof(Shape));
    space->staticShapes = gf2d_list_append(space->staticShapes,(void *)newShape);
}

void gf2d_space_remove_body(Space *space,Body *body)
{
    if (!space)
    {
        slog("no space provided");
        return;
    }
    if (!body)
    {
        slog("no body provided");
        return;
    }
    if (space->bodyList)
    {
        if (gf2d_list_delete_data(space->bodyList,(void *)body)!= 0)
        {
            slog("failed to remove body named %s from the space",body->name);
        }
    }
}

void gf2d_space_add_body(Space *space,Body *body)
{
    if (!space)
    {
        slog("no space provided");
        return;
    }
    if (!body)
    {
        slog("no body provided");
        return;
    }
    space->bodyList = gf2d_list_append(space->bodyList,(void *)body);
}

void gf2d_space_draw(Space *space,Vector2D offset)
{
    int i,count;
    SDL_Rect r;
    if (!space)
    {
        slog("no space provided");
        return;
    }
    r = gf2d_rect_to_sdl_rect(space->bounds);
    vector2d_add(r,r,offset);    
    gf2d_draw_rect(r,vector4d(255,0,0,255));
    count = gf2d_list_get_count(space->bodyList);
    for (i = 0; i < count;i++)
    {
        gf2d_body_draw((Body *)gf2d_list_get_nth(space->bodyList,i),offset);
    }
    count = gf2d_list_get_count(space->staticShapes);
    for (i = 0; i < count;i++)
    {
        gf2d_shape_draw(*(Shape *)gf2d_list_get_nth(space->staticShapes,i),gf2d_color8(0,255,0,255),offset);
    }
}

void gf2d_space_body_pre_step(Body *body,Space *space)
{
    if (!body)return;
    if (vector2d_magnitude_squared(body->velocity)< GF2D_EPSILON)
    {
        body->inactive = 1;
    }
    vector2d_copy(body->newvelocity,body->velocity);
}

void gf2d_space_body_post_step(Body *body,Space *space)
{
    float resistance;
    if (!body)return;
    resistance = 1 - body->elasticity;
    body->newvelocity.x = (body->newvelocity.x * body->elasticity)+(body->velocity.x * resistance);
    body->newvelocity.y = (body->newvelocity.y * body->elasticity)+(body->velocity.y * resistance);
    vector2d_scale(body->newvelocity,body->newvelocity,space->dampening);
    if (vector2d_magnitude_squared(body->newvelocity) < 0.1)vector2d_clear(body->newvelocity);

    vector2d_copy(body->velocity,body->newvelocity);
}

void gf2d_space_body_step(Body *body,Space *space,float step)
{
    int i= 0;
    int attempts = 0;
    int collided = 0;
    Collision collision;
    ClipFilter filter = {0};
    Vector2D poc, normal;
    Vector2D pocB, normalB;
    Vector2D pocS, normalS;
    Uint8 world;
    Body *other,*collider = NULL;
    Shape *shape,*collisionShape = NULL;
    int bodies,staticShapes;
    Vector2D velocity;
    if (!body)return;
    if (!space)return;
    bodies = gf2d_list_get_count(space->bodyList);
    staticShapes = gf2d_list_get_count(space->staticShapes);

    vector2d_scale(velocity,body->velocity,space->timeStep);
    
    vector2d_add(body->position,body->position,velocity);
    if (space->precision)
    {
        vector2d_scale(velocity,velocity,1.0/space->precision);
    }

    filter.layer = body->cliplayer &~ WORLD_LAYER;
    filter.team = body->team;
    world = WORLD_LAYER & body->cliplayer;
    filter.ignore = body;
    
    for (attempts = 0;attempts < space->precision;attempts++)
    {
        //bounds check
        if ((world)&&(gf2d_body_check_bounds(body,space->bounds,&pocB,&normalB)))
        {
            collided = 1;
            goto attempt;
        }
        //body/body collision check
        for (i = 0; i < bodies; i++)
        {
            other = (Body*)gf2d_list_get_nth(space->bodyList,i);
            if ((!other)||// error check
                (other == body))continue;//dont self collide
            if (gf2d_body_collide_filter(body,other,&poc,&normal,filter))
            {
                collider = other;
                goto attempt;
            }
        }
        if (world)
        {
            for (i = 0; i < staticShapes; i++)
            {
                shape = (Shape*)gf2d_list_get_nth(space->staticShapes,i);
                if (!shape)continue;// error check
                if (gf2d_body_shape_collide(body,shape,&pocS,&normalS))
                {
                    collisionShape = shape;
                    goto attempt;
                }
            }
        }
        // this pass collided with no new things
        break;
attempt:
        //collision
        vector2d_sub(body->position,body->position,velocity);
    }
    if (((collider)||(collided))||(collisionShape))
    {
        body->inactive = 1;
    }
    if (collider)
    {
        vector2d_set_magnitude(&velocity,-GF2D_EPSILON);
        vector2d_add(body->position,body->position,velocity);
        if (body->bodyTouch != NULL)
        {
            collision.shape = collider->shape;
            collision.body = collider;
            vector2d_copy(collision.pointOfContact,poc);
            vector2d_copy(collision.normal,normal);
            collision.timeStep = step;
            body->bodyTouch(body,collider,&collision);
        }
        if (collider->bodyTouch != NULL)
        {
            collision.shape = body->shape;
            collision.body = body;
            vector2d_copy(collision.pointOfContact,poc);
            vector2d_copy(collision.normal,normal);
            collision.timeStep = step;
            collider->bodyTouch(collider,body,&collision);
        }

        gf2d_body_adjust_collision_velocity(body,collider,poc, normal);
        gf2d_body_adjust_collision_velocity(collider,body,poc, normal);
    }
    if (collisionShape)
    {
        vector2d_set_magnitude(&velocity,-GF2D_EPSILON);
        vector2d_add(body->position,body->position,velocity);
        if (body->worldTouch != NULL)
        {
            body->worldTouch(body,NULL);
        }
        gf2d_body_adjust_static_bounce_velocity(body,collisionShape,pocS, normalS);
    }
    if (collided)
    {
        if (body->worldTouch != NULL)
        {
            body->worldTouch(body,NULL);
        }
        gf2d_body_adjust_collision_overlap(body,space->slop,space->bounds);
    }
}

void gf2d_space_step(Space *space,float t)
{
    int i= 0;
    Body *body;
    int bodies;
    if (!space)return;
    bodies = gf2d_list_get_count(space->bodyList);
    for (i = 0; i < bodies; i++)
    {
        body = (Body*)gf2d_list_get_nth(space->bodyList,i);
        if (!body)continue;// body already hit something
        gf2d_space_body_pre_step(body,space);
    }
    for (i = 0; i < bodies; i++)
    {
        body = (Body*)gf2d_list_get_nth(space->bodyList,i);
        if ((!body)||(body->inactive))continue;// body already hit something
        gf2d_space_body_step(body,space,t);
    }
    for (i = 0; i < bodies; i++)
    {
        body = (Body*)gf2d_list_get_nth(space->bodyList,i);
        if (!body)continue;// body already hit something
        gf2d_space_body_post_step(body,space);
    }
}

void gf2d_space_update(Space *space)
{
    int i;
    float s;
    Body *body;
    int bodies;
    if (!space)return;
    // reset all body tracking
    bodies = gf2d_list_get_count(space->bodyList);
    for (i = 0; i < bodies;i ++)
    {
        body = (Body*)gf2d_list_get_nth(space->bodyList,i);
        if (!body)continue;
        body->inactive = 0;
    }
    for (s = 0; s < 1; s += space->timeStep)
    {
        gf2d_space_step(space,s);
    }
}

void gf2d_space_body_collision_test_filter(Space *space,Shape shape, Collision *collision,ClipFilter filter)
{
    int i,count;
    Body *body;
    if ((!space)||(!collision))return;
    memset(collision,0,sizeof(Collision));
    count = gf2d_list_get_count(space->bodyList);
    for (i = 0; i < count; i++)
    {
        body = (Body*)gf2d_list_get_nth(space->bodyList,i);
        if (!body)continue;
        if (!(body->cliplayer & filter.layer))continue;
        if ((filter.team)&&(body->team == filter.team))continue;
        if (body == filter.ignore)continue;
        if(gf2d_body_shape_collide(body,&shape,&collision->pointOfContact, &collision->normal))
        {
            collision->collided = 1;
            collision->body = body;
            collision->shape = body->shape;
            return;
        }
    }
}


/*eol@eof*/

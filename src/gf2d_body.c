#include "gf2d_body.h"
#include "gf2d_draw.h"
#include "simple_logger.h"

Uint8 gf2d_body_shape_collide(Body *a,Shape *s,Vector2D *poc, Vector2D *normal);

void gf2d_body_clear(Body *body)
{
    if (!body)return;
    memset(body,0,sizeof(Body));
}

void gf2d_body_push(Body *body,Vector2D direction,float force)
{
    if (!body)return;
    if (body->mass != 0)
    {
        force = force / body->mass;
    }
    vector2d_set_magnitude(&direction,force);
    vector2d_add(body->velocity,body->velocity,direction);
}

void gf2d_body_draw(Body *body,Vector2D offset)
{
    Vector4D color;
    Shape shape;
    Vector2D center;
    if (!body)return;
    vector4d_set(color,0,255,255,255);
    // draw center point
    vector2d_add(center,body->position,offset);
    gf2d_draw_pixel(center,color);
        
    vector4d_set(color,255,0,255,255);
    gf2d_shape_copy(&shape,*body->shape);
    gf2d_shape_move(&shape,body->position);
    gf2d_shape_draw(shape,gf2d_color_from_vector4(color),offset);
}

void gf2d_body_set(
    Body       *body,
    char       *name,
    Uint32      cliplayer,
    Uint32      touchlayer,
    Uint32      team,
    Vector2D    position,
    Vector2D    velocity,
    float       mass,
    float       gravity,
    float       elasticity,
    Shape      *shape,
    void       *data,
    int     (*bodyTouch)(struct Body_S *self, struct Body_S *other, Collision *collision),
    int     (*worldTouch)(struct Body_S *self, Collision *collision))
{
    if (!body)return;
    body->cliplayer = cliplayer;
    body->touchlayer = touchlayer;
    body->team = team;
    vector2d_copy(body->position,position);
    vector2d_copy(body->velocity,velocity);
    body->mass = mass;
    body->gravity = gravity;
    body->elasticity = elasticity;
    body->shape = shape;
    body->data = data;
    body->bodyTouch = bodyTouch;
    body->worldTouch = worldTouch;
    gf2d_word_cpy(body->name,name);
}

Vector2D gf2d_body_normal(Body *body,Vector2D poc, Vector2D *normal)
{
    Vector2D n = {0,0};
    // given the point of contact make sure the normal points Away from the body
    if (!body)return n;
    if (!normal)
    {
        // calculate based on relative position of POC
        n.x = poc.x - body->position.x;
        n.y = poc.y - body->position.y;
        vector2d_normalize(&n);
        return n;
    }
    vector2d_copy(n,(*normal));
    if ((normal->x > 0) && (body->position.x < poc.x))
    {
        n = vector2d_rotate(n,M_PI);
        return n;
    }
    if ((normal->x < 0) && (body->position.x > poc.x))
    {
        n = vector2d_rotate(n,M_PI);
        return n;
    }
    if ((normal->y > 0) && (body->position.y < poc.y))
    {
        n = vector2d_rotate(n,M_PI);
        return n;
    }
    if ((normal->y < 0) && (body->position.y > poc.y))
    {
        n = vector2d_rotate(n,M_PI);
        return n;
    }
    return n;
}

void gf2d_body_adjust_bounds_collision_velocity(Body *a,Vector2D poc, Vector2D normal)
{
    Vector2D epsilon;

    normal = gf2d_body_normal(a,poc, &normal);

    vector2d_copy(epsilon,normal);
    vector2d_set_magnitude(&epsilon,GF2D_EPSILON);
    vector2d_add(a->position,a->position,epsilon);
    if (normal.x > GF2D_EPSILON)
    {
        a->newvelocity.x = fabs(a->newvelocity.x);
    }
    else if (normal.x < -GF2D_EPSILON)
    {
        a->newvelocity.x = -fabs(a->newvelocity.x);
    }
    if (normal.y > GF2D_EPSILON)
    {
        a->newvelocity.y = fabs(a->newvelocity.y);
    }
    else if (normal.y < -GF2D_EPSILON)
    {
        a->newvelocity.y = -fabs(a->newvelocity.y);
    }
}

void gf2d_body_adjust_static_bounce_velocity(Body *a,Shape *s,Vector2D poc, Vector2D normal)
{
    double phi;//contact angle
    double theta1,theta2;// movement angles
    double v1,v2,part1;
    Vector2D nv;
    if ((!a)||(!s))return;
    normal = gf2d_body_normal(a,poc, &normal);
    v1 = vector2d_magnitude(a->velocity);
    v2 = 0;
    theta1 = vector2d_angle(a->velocity)*GF2D_DEGTORAD;
    theta2 = 0;
    phi = vector2d_angle(normal)*GF2D_DEGTORAD;
    part1 = (v1*cos(theta1-phi)*(a->mass - 100000) + 2 *100000*v2*cos(theta2 - phi))/(a->mass+100000);
    nv.x = part1*cos(phi)+v1*sin(theta1-phi)*cos(phi + GF2D_HALF_PI);
    nv.y = part1*sin(phi)+v1*sin(theta1-phi)*sin(phi + GF2D_HALF_PI);
    vector2d_copy(a->newvelocity,nv);
}

void gf2d_body_adjust_collision_overlap(Body *a,float slop,Rect bounds)
{
    Rect r;
    if (!a)return;
    r = gf2d_shape_get_bounds(*a->shape);
    vector2d_add(r,r,a->position);
    if (r.x < bounds.x)r.x = bounds.x + slop;
    if (r.y < bounds.y)r.y = bounds.y + slop;
    if (r.x + r.w > bounds.x + bounds.w)r.x = bounds.x + bounds.w - r.w - slop;
    if (r.y + r.h > bounds.y + bounds.h)r.y = bounds.y + bounds.h - r.h - slop;
}

void gf2d_body_adjust_collision_bounds_velocity(Body *a,float slop, Rect bounds,Vector2D *velocity)
{
    Rect r;
    if (!a)return;
    r = gf2d_shape_get_bounds(*a->shape);
    vector2d_add(r,r,a->position);
    vector2d_add(r,r,(*velocity));
    if ((r.x <= bounds.x + slop)&&(velocity->x < 0))velocity->x = 0;
    if ((r.y <= bounds.y + 2)&&(velocity->y < 0))
    {
        slog("would collide with upper bounds");
        velocity->y = 0;
    }
    if ((r.x + r.w >= bounds.x + bounds.w - slop)&&(velocity->x > 0))velocity->x = 0;
    if ((r.y + r.h >= bounds.y + bounds.h - slop)&&(velocity->y > 0))
    {
        velocity->y = 0;
        slog("would collide with lower bounds");
    }
}


void gf2d_body_adjust_collision_velocity(Body *a,Body *b,Vector2D poc, Vector2D normal)
{
    double phi;//contact angle
    double theta1,theta2;// movement angles
    double v1,v2;
    double part1;
    Vector2D nv;
    if ((!a)||(!b))return;
    normal = gf2d_body_normal(a,poc, &normal);
    v1 = vector2d_magnitude(a->velocity);
    if (b->inactive)
    {
        v2 = 0;
    }
    else v2 = vector2d_magnitude(b->velocity);
    theta1 = vector2d_angle(a->velocity)*GF2D_DEGTORAD;
    theta2 = vector2d_angle(b->velocity)*GF2D_DEGTORAD;
    phi = vector2d_angle(normal)*GF2D_DEGTORAD;
    part1 = (v1*cos(theta1-phi)*(a->mass - b->mass) + 2 *b->mass*v2*cos(theta2 - phi))/(a->mass+b->mass);
    nv.x = part1*cos(phi)+v1*sin(theta1-phi)*cos(phi + GF2D_HALF_PI);
    nv.y = part1*sin(phi)+v1*sin(theta1-phi)*sin(phi + GF2D_HALF_PI);
    vector2d_copy(a->newvelocity,nv);
}

Shape gf2d_body_to_shape(Body *a)
{
    Shape aS = {0};
    if (!a)return aS;
    gf2d_shape_copy(&aS,*a->shape);
    gf2d_shape_move(&aS,a->position);
    return aS;
}

Uint8 gf2d_body_shape_collide(Body *a,Shape *s,Vector2D *poc, Vector2D *normal)
{
    Shape aS;
    if ((!a)||(!s))return 0;
    // set shapes based on each body's current position
    aS = gf2d_body_to_shape(a);
    
    return gf2d_shape_overlap_poc(aS, *s,poc,normal);
}

Uint8 gf2d_body_collide_filter(Body *a,Body *b,Vector2D *poc, Vector2D *normal, ClipFilter filter)
{
    Shape aS,bS;
    if ((!a)||(!b))return 0;
    // set shapes based on each body's current position
    if (!(b->cliplayer & filter.layer))return 0; // no common layers
    if ((filter.team)&&(b->team == filter.team)) return 0;  // same team
    if (b == filter.ignore) return 0;
    gf2d_shape_copy(&aS,*a->shape);
    gf2d_shape_move(&aS,a->position);

    gf2d_shape_copy(&bS,*b->shape);
    gf2d_shape_move(&bS,b->position);
    
    return gf2d_shape_overlap_poc(aS, bS,poc,normal);
}

Uint8 gf2d_body_collide(Body *a,Body *b,Vector2D *poc, Vector2D *normal)
{
    ClipFilter filter = {0};
    return gf2d_body_collide_filter(a,b,poc, normal, filter);
}

Uint8 gf2d_body_check_bounds(Body *body,Rect bounds,Vector2D *poc,Vector2D *normal)
{
    Shape aS= {0},bs={0};
    if (!body)return 0;
    bs = gf2d_shape_from_rect(bounds);

    // set shapes based on each body's current position
    gf2d_shape_copy(&aS,*body->shape);
    gf2d_shape_move(&aS,body->position);
    
    if (!gf2d_shape_overlap(aS, bs))
    {// we should DEFINITELY be inside the bounds, so return true here
        return 1;
    }
    if (gf2d_edge_intersect_shape_poc(gf2d_edge(bounds.x,bounds.y,bounds.x+bounds.w,bounds.y),aS,poc,normal)||
        gf2d_edge_intersect_shape_poc(gf2d_edge(bounds.x,bounds.y,bounds.x,bounds.y+bounds.h),aS,poc,normal)||
        gf2d_edge_intersect_shape_poc(gf2d_edge(bounds.x,bounds.y+bounds.h,bounds.x+bounds.w,bounds.y+bounds.h),aS,poc,normal)||
        gf2d_edge_intersect_shape_poc(gf2d_edge(bounds.x+bounds.w,bounds.y,bounds.x+bounds.w,bounds.y+bounds.h),aS,poc,normal))
    {
        return 1;
    }
    return 0;
}

/*eol@eof*/

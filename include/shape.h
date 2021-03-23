#ifndef __SHAPE_H__
#define __SHAPE_H__

#include "gfc_types.h"
#include "gfc_vector.h"

typedef struct
{
	float x1, y1, x2, y2;
}Edge;

typedef struct
{
	float x, y, w, h;
}ShapeRect;

typedef struct
{
	float x, y, r;
}ShapeCircle;

typedef enum
{
	ST_RECT,
	ST_CIRCLE,
	ST_EDGE
}ShapeTypes;

typedef struct
{
	ShapeTypes type;
	union
	{
		ShapeCircle c;
		ShapeRect r;
		Edge e;
	}s;
}Shape;

typedef Vector2D ShapePoint;

ShapeRect gf2d_rect(float x, float y, float w, float h);
Shape gf2d_shape_rect(float x, float y, float w, float h);
Shape gf2d_shape_from_rect(ShapeRect r);
Shape gf2d_shape_from_sdl_rect(SDL_Rect r);
Shape gf2d_shape_circle(float x, float y, float r);
Shape gf2d_shape_from_circle(ShapeCircle c);
Shape gf2d_shape_edge(float x1, float y1, float x2, float y2);
Shape gf2d_shape_from_edge(Edge e);
#ifndef gf2d_rect_set
#define gf2d_rect_set(r,a,b,c,d) (r.x = a,r.y = b, r.w =c, r.h = d)
#endif
#define gf2d_rect_copy(dst,src) (dst.x = src.x,dst.y = src.y,dst.w = src.w,dst.h = src.h)
Uint8 gf2d_point_in_rect(Vector2D p, ShapeRect r);
Uint8 gf2d_rect_overlap(ShapeRect a,ShapeRect b);
Uint8 gf2d_rect_overlap_poc(ShapeRect a, ShapeRect b, Vector2D *poc, Vector2D *normal);
ShapeCircle gf2d_circle(float x, float y, float r);
#define gf2d_circle_set(circle,a,b,c) (circle.x = a,circle.y = b, circle.r = c)
Uint8 gf2d_point_in_cicle(Vector2D p, ShapeCircle c);
Uint8 gf2d_circle_overlap(ShapeCircle a, ShapeCircle b);
Uint8 gf2d_circle_overlap_poc(ShapeCircle a, ShapeCircle b, Vector2D *poc, Vector2D *normal);
Uint8 gf2d_circle_rect_overlap_poc(ShapeCircle a, ShapeRect b, Vector2D *poc, Vector2D * normal);
Uint8 gf2d_circle_rect_overlap(ShapeCircle a, ShapeRect b);
Uint8 gf2d_shape_overlap_poc(Shape a, Shape b, Vector2D *poc, Vector2D *normal);
SDL_Rect gf2d_rect_to_sdl_rect(ShapeRect r);
ShapeRect gf2d_rect_from_sdl_rect(SDL_Rect r);
void gf2d_shape_move(Shape *shape, Vector2D move);
void gf2d_shape_copy(Shape *dst, Shape src);
Edge gf2d_edge(float x1, float y1, float x2, float y2);
Edge gf2d_edge_from_vectors(Vector2D a, Vector2D b);
#define gf2d_edge_set(e,a,b,c,d) (e.x1 = a,e.y1 = b, e.x2 = c, e.y2 = d)
#define gf2d_edge_copy(dst,src) (dst.x1 = src.x1,dst.y1 = src.y1,dst.x2 = src.x2,dst.y2 = src.y2)
Uint8 gf2d_edge_intersect_poc(
	Edge a,
	Edge b,
	Vector2D *contact,
	Vector2D *normal);
Uint8 gf2d_edge_intersect(Edge a, Edge b);
Uint8 gf2d_edge_rect_intersection(Edge e, ShapeRect r);
Uint8 gf2d_edge_rect_intersection_poc(Edge e, ShapeRect r, Vector2D *poc, Vector2D *normal);
Uint8 gf2d_edge_circle_intersection(Edge e, ShapeCircle c);
Uint8 gf2d_edge_circle_intersection_poc(Edge e, ShapeCircle c, Vector2D *poc, Vector2D *normal);
Uint8 gf2d_edge_intersect_shape(Edge e, Shape s);
Uint8 gf2d_edge_intersect_shape_poc(Edge e, Shape s, Vector2D *poc, Vector2D *normal);
void gf2d_shape_slog(Shape shape);
void gf2d_rect_slog(ShapeRect r);
ShapeRect gf2d_shape_get_bounds(Shape shape);




ShapeRect shape_rect_from_sdl_rect(SDL_Rect r);
ShapeRect shape_rect_from_vector4d(Vector4D v);
SDL_Rect shape_rect_to_sdl_rect(ShapeRect r);

/**
* @brief check if the two circles have overlap
* @param a the first circle to check
* @param b the second circle to check
* @return true if there is overlap, false if there is not
*/
Bool shape_circle_collision(ShapeCircle a, ShapeCircle b);

/**
* @brief check if a given point is inside a given circle
* @param point the point in question
* @param c the circle in question
* @return true if there is overlap, false if there is not
*/
Bool shape_point_in_circle(ShapePoint point, ShapeCircle c);

/**
* @brief check if a given point is inside a given rectangle
* @param point the point in question
* @param rect the rectangle in question
* @return true if there is overlap, false if there is not
*/
Bool shape_point_in_rect(ShapePoint point, ShapeRect rect);


/**
* @brief check if the two rectangles have overlap
* @param a the first rectangle to check
* @param b the second rectangle to check
* @return true if there is overlap, false if there is not
*/
Bool shape_rect_collision(ShapeRect a, ShapeRect b);


#endif
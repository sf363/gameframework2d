#ifndef __GF2D_GRAPHICS_C__
#define __GF2D_GRAPHICS_C__

#include <SDL.h>

#include "gfc_types.h"
#include "gfc_vector.h"

void gf2d_graphics_initialize(
	char *windowName,
	int viewWidth,
	int viewHeight,
	int renderWidth,
	int renderHeight,
	Vector4D bgcolor,
	Bool fullscreen
	);

void gf2d_graphics_set_frame_delay(Uint32 frameDelay);

float gf2d_graphics_get_frames_per_second();

SDL_Renderer *gf2d_graphics_get_renderer();

void gf2d_grahics_next_frame();

void gf2d_graphics_clear_screen();

SDL_Surface *gf2d_graphics_create_surface(Uint32 w, Uint32 h);

void gf2d_graphics_blit_surface_to_screen(SDL_Surface *surface, const SDL_Rect * srcRect, SDL_Rect * dstRect);

SDL_Surface *gf2d_graphics_screen_convert(SDL_Surface **surface);

#endif
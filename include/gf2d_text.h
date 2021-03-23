#ifndef __MGL_TEXT__H__
#define __MGL_TEXT__H__

#include "gfc_types.h"
#include "gfc_color.h"

typedef char TextWord[16];
#define GF2DWORDLEN   16
#define gf2d_word_cmp(a,b) (strncmp(a,b,GF2DWORDLEN))
#define gf2d_word_cpy(dst,src) (strncpy(dst,src,GF2DWORDLEN))
#define gf2d_word_clear(a)  (memset(a,0,sizeof(char)*GF2DWORDLEN))

typedef char TextLine[128];
#define GF2DLINELEN   128
#define gf2d_line_cmp(a,b) (strncmp(a,b,GF2DLINELEN))
#define gf2d_line_cpy(dst,src) (strncpy(dst,src,GF2DLINELEN))
#define gf2d_line_clear(a)  (memset(a,0,sizeof(char)*GF2DLINELEN))
#define gfc_line_sprintf(a,...) snprintf(a,GFCLINELEN,__VA_ARGS__)

typedef char TextBlock[512];
#define GF2DTEXTLEN   512
#define gf2d_block_cmp(a,b) (strncmp(a,b,GF2DTEXTLEN))
#define gf2d_block_cpy(dst,src) (strncpy(dst,src,GF2DTEXTLEN))
#define gf2d_block_clear(a)  (memset(a,0,sizeof(char)*GF2DTEXTLEN))

typedef enum
{
	FT_Normal,
	FT_Small,
	FT_H1,
	FT_H2,
	FT_H3,
	FT_H4,
	FT_H5,
	FT_H6,
	FT_MAX
}FontTypes;

void gf2d_text_init(char *configFile);

void gf2d_text_draw_line_font(char *text, char *filename, Color color, Vector2D position);

void gf2d_text_draw_line(char *text, FontTypes tag, Color color, Vector2D position);

#endif
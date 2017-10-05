#include "tilemap.h"

TileMap *tilemap_load(char *filename);

void tilemap_free(TileMap *tilemap)
{
    if (!tilemap)return;
    if (tilemap->map)
    {
        free(tilemap->map);
    }
    gf2d_sprite_free(tilemap->tileset);
    free(tilemap);
}

void tilemap_draw(TileMap *tilemap,Vector2D position)
{
    if (!tilemap)return;
    if (!tilemap->map)return;
    if (!tilemap->tileset)return;
    
}

void tilemap_draw_path(Vector2D *path,Vector2D position);



/*eol@eof*/

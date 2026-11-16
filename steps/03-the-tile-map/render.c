#include "render.h"

void    render_map(t_map const *map, SDL_Renderer *ren,
        SDL_Texture *tileset, int camera_x)
{
    int         first_col;
    int         last_col;
    int         x;
    int         y;
    int         tile;
    SDL_Rect    src;
    SDL_Rect    dst;

    first_col = tile_index(camera_x);
    last_col = tile_index(camera_x + WINDOW_W) + 1;
    if (last_col > map->width)
        last_col = map->width;
    y = 0;
    while (y < map->height) {
        x = first_col;
        while (x < last_col) {
            tile = map_tile_at(map, x, y);
            if (tile != TILE_EMPTY) {
                src.x = (tile - 1) * TILE_SIZE;
                src.y = 0;
                src.w = TILE_SIZE;
                src.h = TILE_SIZE;
                dst.x = x * TILE_SIZE - camera_x;
                dst.y = y * TILE_SIZE;
                dst.w = TILE_SIZE;
                dst.h = TILE_SIZE;
                SDL_RenderCopy(ren, tileset, &src, &dst);
            }
            x++;
        }
        y++;
    }
}


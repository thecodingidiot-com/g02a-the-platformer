#include "map.h"

int map_load(t_map *map, char const *path)
{
    (void)map;
    (void)path;
    return (0);
}

void map_free(t_map *map)
{
    (void)map;
}

int map_tile_at(t_map const *map, int tile_x, int tile_y)
{
    (void)map;
    (void)tile_x;
    (void)tile_y;
    return (TILE_EMPTY);
}

int map_is_solid(t_map const *map, int tile_x, int tile_y)
{
    (void)map;
    (void)tile_x;
    (void)tile_y;
    return (0);
}

int tile_index(int pixel)
{
    (void)pixel;
    return (0);
}

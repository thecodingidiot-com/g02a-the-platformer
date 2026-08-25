#ifndef MAP_H
# define MAP_H

# define TILE_SIZE      32
# define TILE_EMPTY     0
# define TILE_GROUND    1
# define TILE_PLATFORM  2
# define WINDOW_W       800
# define WINDOW_H       608

typedef struct s_map
{
    int     width;
    int     height;
    int     *tiles;
    int     start_x;
    int     start_y;
    int     goal_x;
    int     goal_y;
}   t_map;

int     map_load(t_map *map, char const *path);
void    map_free(t_map *map);
int     map_tile_at(t_map const *map, int tile_x, int tile_y);
int     map_is_solid(t_map const *map, int tile_x, int tile_y);

#endif

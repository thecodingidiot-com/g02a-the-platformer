#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include "libtci.h"
#include "map.h"

static void trim_newline(char *line)
{
    size_t  len;

    len = tci_strlen(line);
    if (len > 0 && line[len - 1] == '\n')
        line[len - 1] = '\0';
}

static int  parse_dimensions(int fd, int *width, int *height)
{
    char    *line;
    char    *space;

    line = tci_getline(fd);
    if (!line)
        return (0);
    trim_newline(line);
    space = tci_strchr(line, ' ');
    if (!space) {
        free(line);
        return (0);
    }
    *space = '\0';
    *width = tci_atoi(line);
    *height = tci_atoi(space + 1);
    free(line);
    return (1);
}

static int  tile_for_char(char c, t_map *map, int x, int y)
{
    if (c == '#')
        return (TILE_GROUND);
    if (c == '=')
        return (TILE_PLATFORM);
    if (c == 'S') {
        map->start_x = x * TILE_SIZE;
        map->start_y = y * TILE_SIZE;
        return (TILE_EMPTY);
    }
    if (c == 'G') {
        map->goal_x = x * TILE_SIZE;
        map->goal_y = y * TILE_SIZE;
        return (TILE_EMPTY);
    }
    return (TILE_EMPTY);
}

int map_load(t_map *map, char const *path)
{
    int     fd;
    int     y;
    int     x;
    char    *line;

    fd = open(path, O_RDONLY);
    if (fd < 0)
        return (0);
    if (!parse_dimensions(fd, &map->width, &map->height)) {
        close(fd);
        return (0);
    }
    map->tiles = malloc(sizeof(int) * map->width * map->height);
    if (!map->tiles) {
        close(fd);
        return (0);
    }
    map->start_x = 0;
    map->start_y = 0;
    map->goal_x = 0;
    map->goal_y = 0;
    y = 0;
    while (y < map->height) {
        line = tci_getline(fd);
        if (!line) {
            close(fd);
            return (0);
        }
        trim_newline(line);
        x = 0;
        while (x < map->width) {
            map->tiles[y * map->width + x] = tile_for_char(line[x], map, x, y);
            x++;
        }
        free(line);
        y++;
    }
    close(fd);
    return (1);
}

void    map_free(t_map *map)
{
    free(map->tiles);
    map->tiles = NULL;
}

int map_tile_at(t_map const *map, int tile_x, int tile_y)
{
    if (tile_x < 0 || tile_x >= map->width)
        return (TILE_GROUND);
    if (tile_y < 0 || tile_y >= map->height)
        return (TILE_GROUND);
    return (map->tiles[tile_y * map->width + tile_x]);
}

int map_is_solid(t_map const *map, int tile_x, int tile_y)
{
    int tile;

    tile = map_tile_at(map, tile_x, tile_y);
    return (tile == TILE_GROUND || tile == TILE_PLATFORM);
}

int tile_index(int pixel)
{
    if (pixel < 0)
        return ((pixel - (TILE_SIZE - 1)) / TILE_SIZE);
    return (pixel / TILE_SIZE);
}

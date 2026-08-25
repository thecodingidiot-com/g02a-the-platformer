#include "camera.h"

void    camera_init(t_camera *cam)
{
    cam->x = 0;
}

void    camera_update(t_camera *cam, float player_x, t_map const *map)
{
    int target;
    int max_x;

    target = (int)player_x - WINDOW_W / 2;
    max_x = map->width * TILE_SIZE - WINDOW_W;
    if (max_x < 0)
        max_x = 0;
    if (target < 0)
        target = 0;
    if (target > max_x)
        target = max_x;
    cam->x = target;
}

int camera_world_to_screen_x(t_camera const *cam, int world_x)
{
    return (world_x - cam->x);
}

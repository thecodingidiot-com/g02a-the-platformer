#include "camera.h"

void camera_init(t_camera *cam)
{
    (void)cam;
}

void camera_update(t_camera *cam, float player_x, t_map const *map)
{
    (void)cam;
    (void)player_x;
    (void)map;
}

int camera_world_to_screen_x(t_camera const *cam, int world_x)
{
    (void)cam;
    (void)world_x;
    return (0);
}

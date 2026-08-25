#ifndef CAMERA_H
# define CAMERA_H

# include "map.h"

typedef struct s_camera
{
    int     x;
}   t_camera;

void    camera_init(t_camera *cam);
void    camera_update(t_camera *cam, float player_x, t_map const *map);
int     camera_world_to_screen_x(t_camera const *cam, int world_x);

#endif

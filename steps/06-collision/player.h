#ifndef PLAYER_H
# define PLAYER_H

# include <SDL2/SDL.h>
# include "map.h"

# define PLAYER_WIDTH    28
# define PLAYER_HEIGHT   44
# define MOVE_SPEED      4.0f
# define GRAVITY         0.6f
# define JUMP_SPEED      -12.0f
# define MAX_FALL_SPEED  12.0f

typedef enum e_anim
{
    ANIM_IDLE,
    ANIM_WALK,
    ANIM_JUMP
}   t_anim;

typedef struct s_player
{
    float           x;
    float           y;
    float           vx;
    float           vy;
    int             on_ground;
    int             facing;
    t_anim          anim;
    int             frame;
    int             frame_timer;
}   t_player;

void    player_init(t_player *p, float x, float y);
void    player_handle_input(t_player *p, Uint8 const *keys);
void    player_update_physics(t_player *p);
void    player_resolve_collision(t_player *p, t_map const *map);
void    player_update_animation(t_player *p);
int     player_sheet_frame(t_player const *p);

#endif

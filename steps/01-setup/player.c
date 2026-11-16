#include "player.h"

void player_init(t_player *p, float x, float y)
{
    (void)p;
    (void)x;
    (void)y;
}

void player_handle_input(t_player *p, Uint8 const *keys)
{
    (void)p;
    (void)keys;
}

void player_update_physics(t_player *p)
{
    (void)p;
}

void player_resolve_collision(t_player *p, t_map const *map)
{
    (void)p;
    (void)map;
}

void player_update_animation(t_player *p)
{
    (void)p;
}

int player_sheet_frame(t_player const *p)
{
    (void)p;
    return (0);
}

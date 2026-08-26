#include "player.h"

void    player_init(t_player *p, float x, float y)
{
    p->x = x;
    p->y = y;
    p->vx = 0.0f;
    p->vy = 0.0f;
    p->on_ground = 0;
    p->facing = 1;
    p->anim = ANIM_IDLE;
    p->frame = 0;
    p->frame_timer = 0;
}

void    player_handle_input(t_player *p, Uint8 const *keys)
{
    p->vx = 0.0f;
    if (keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_H])
    {
        p->vx = -MOVE_SPEED;
        p->facing = -1;
    }
    if (keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_L])
    {
        p->vx = MOVE_SPEED;
        p->facing = 1;
    }
    if ((keys[SDL_SCANCODE_SPACE] || keys[SDL_SCANCODE_K]) && p->on_ground)
    {
        p->vy = JUMP_SPEED;
        p->on_ground = 0;
    }
}

void    player_update_physics(t_player *p)
{
    p->vy += GRAVITY;
    if (p->vy > MAX_FALL_SPEED)
        p->vy = MAX_FALL_SPEED;
}

static void move_x(t_player *p, t_map const *map)
{
    int left_tile;
    int right_tile;
    int top_tile;
    int bottom_tile;

    p->x += p->vx;
    left_tile = tile_index((int)p->x);
    right_tile = tile_index((int)(p->x + PLAYER_WIDTH - 1));
    top_tile = tile_index((int)p->y);
    bottom_tile = tile_index((int)(p->y + PLAYER_HEIGHT - 1));
    if (p->vx > 0 && (map_is_solid(map, right_tile, top_tile)
            || map_is_solid(map, right_tile, bottom_tile)))
    {
        p->x = (float)(right_tile * TILE_SIZE - PLAYER_WIDTH);
        p->vx = 0.0f;
    }
    else if (p->vx < 0 && (map_is_solid(map, left_tile, top_tile)
            || map_is_solid(map, left_tile, bottom_tile)))
    {
        p->x = (float)((left_tile + 1) * TILE_SIZE);
        p->vx = 0.0f;
    }
}

static void move_y(t_player *p, t_map const *map)
{
    int left_tile;
    int right_tile;
    int top_tile;
    int bottom_tile;

    p->y += p->vy;
    left_tile = tile_index((int)p->x);
    right_tile = tile_index((int)(p->x + PLAYER_WIDTH - 1));
    top_tile = tile_index((int)p->y);
    bottom_tile = tile_index((int)(p->y + PLAYER_HEIGHT - 1));
    if (p->vy > 0 && (map_is_solid(map, left_tile, bottom_tile)
            || map_is_solid(map, right_tile, bottom_tile)))
    {
        p->y = (float)(bottom_tile * TILE_SIZE - PLAYER_HEIGHT);
        p->vy = 0.0f;
    }
    else if (p->vy < 0 && (map_is_solid(map, left_tile, top_tile)
            || map_is_solid(map, right_tile, top_tile)))
    {
        p->y = (float)((top_tile + 1) * TILE_SIZE);
        p->vy = 0.0f;
    }
}

static int  probe_ground(t_player const *p, t_map const *map)
{
    int left_tile;
    int right_tile;
    int probe_tile;

    left_tile = tile_index((int)p->x);
    right_tile = tile_index((int)(p->x + PLAYER_WIDTH - 1));
    probe_tile = tile_index((int)(p->y + PLAYER_HEIGHT));
    return (map_is_solid(map, left_tile, probe_tile)
            || map_is_solid(map, right_tile, probe_tile));
}

void    player_resolve_collision(t_player *p, t_map const *map)
{
    move_x(p, map);
    move_y(p, map);
    p->on_ground = (p->vy >= 0.0f) && probe_ground(p, map);
}

void    player_update_animation(t_player *p)
{
    if (!p->on_ground)
    {
        p->anim = ANIM_JUMP;
        p->frame = 0;
        p->frame_timer = 0;
    }
    else if (p->vx != 0.0f)
    {
        p->anim = ANIM_WALK;
        p->frame_timer++;
        if (p->frame_timer >= 8)
        {
            p->frame_timer = 0;
            p->frame = 1 - p->frame;
        }
    }
    else
    {
        p->anim = ANIM_IDLE;
        p->frame = 0;
        p->frame_timer = 0;
    }
}

int player_sheet_frame(t_player const *p)
{
    if (p->anim == ANIM_JUMP)
        return (3);
    if (p->anim == ANIM_WALK)
        return (1 + p->frame);
    return (0);
}

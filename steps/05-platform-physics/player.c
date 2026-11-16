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
    if (keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_H]) {
        p->vx = -MOVE_SPEED;
        p->facing = -1;
    }
    if (keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_L]) {
        p->vx = MOVE_SPEED;
        p->facing = 1;
    }
    if (keys[SDL_SCANCODE_SPACE] && p->on_ground) {
        p->vy = JUMP_SPEED;
        p->on_ground = 0;
    }
}

void    player_update_physics(t_player *p)
{
    p->vy += GRAVITY;
    if (p->vy > MAX_FALL_SPEED)
        p->vy = MAX_FALL_SPEED;
    p->x += p->vx;
    p->y += p->vy;
}

void    player_update_animation(t_player *p)
{
    if (!p->on_ground) {
        p->anim = ANIM_JUMP;
        p->frame = 0;
        p->frame_timer = 0;
    } else if (p->vx != 0.0f) {
        p->anim = ANIM_WALK;
        p->frame_timer++;
        if (p->frame_timer >= 8) {
            p->frame_timer = 0;
            p->frame = 1 - p->frame;
        }
    } else {
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

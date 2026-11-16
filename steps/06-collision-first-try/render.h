#ifndef RENDER_H
# define RENDER_H

# include <SDL2/SDL.h>
# include "map.h"
# include "player.h"

void    render_map(t_map const *map, SDL_Renderer *ren,
        SDL_Texture *tileset, int camera_x);
void    render_player(t_player const *p, SDL_Renderer *ren,
        SDL_Texture *spritesheet, int camera_x);

#endif

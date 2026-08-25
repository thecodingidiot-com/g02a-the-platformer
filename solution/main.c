#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "libtci.h"
#include "map.h"
#include "player.h"
#include "camera.h"
#include "render.h"

int main(int argc, char **argv)
{
    SDL_Window      *win;
    SDL_Renderer    *ren;
    SDL_Event       ev;
    SDL_Texture     *tileset;
    SDL_Texture     *spritesheet;
    t_map           map;
    t_player        player;
    t_camera        camera;
    Uint8 const     *keys;
    int             running;

    if (argc < 2)
    {
        tci_printf("usage: %s <map_file>\n", argv[0]);
        return (1);
    }
    if (!map_load(&map, argv[1]))
    {
        tci_printf("failed to load map: %s\n", argv[1]);
        return (1);
    }
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_Log("SDL_Init: %s", SDL_GetError());
        return (1);
    }
    win = SDL_CreateWindow("g02a", SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, WINDOW_W, WINDOW_H, 0);
    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    IMG_Init(IMG_INIT_PNG);
    tileset = IMG_LoadTexture(ren, "assets/tileset.png");
    spritesheet = IMG_LoadTexture(ren, "assets/spritesheet.png");
    player_init(&player, (float)map.start_x, (float)map.start_y);
    camera_init(&camera);
    running = 1;
    while (running)
    {
        while (SDL_PollEvent(&ev))
        {
            if (ev.type == SDL_QUIT)
                running = 0;
            if (ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_ESCAPE)
                running = 0;
        }
        keys = SDL_GetKeyboardState(NULL);
        player_handle_input(&player, keys);
        player_update_physics(&player);
        player_resolve_collision(&player, &map);
        player_update_animation(&player);
        camera_update(&camera, player.x, &map);
        SDL_SetRenderDrawColor(ren, 100, 149, 237, 255);
        SDL_RenderClear(ren);
        render_map(&map, ren, tileset, camera.x);
        render_player(&player, ren, spritesheet, camera.x);
        SDL_RenderPresent(ren);
        SDL_Delay(16);
    }
    SDL_DestroyTexture(tileset);
    SDL_DestroyTexture(spritesheet);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    IMG_Quit();
    SDL_Quit();
    map_free(&map);
    return (0);
}

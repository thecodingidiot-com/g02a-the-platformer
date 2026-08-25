# g02a-the-platformer

Companion repository for **g02a — The Platformer** at
[thecodingidiot.com](https://thecodingidiot.com).

---

## Follow my journey

Working through g02a alongside the implementation pages? Build
`platformer` step by step, then run the tester.

Clone this repository — it carries everything needed, including the
`libtci/` subdirectory with all library source:

```bash
git clone https://github.com/thecodingidiot-com/g02a-the-platformer.git g02a-practice
cd g02a-practice/solution
make -C libtci re
bash gen_assets.sh
make re
bash ../test.sh
```

All tests must pass before the chapter is complete.

---

## Follow your journey

Building `platformer` independently? Here is the full project brief.

A 2D side-scrolling platformer:

- A tile map, loaded from a plain-text file (`width height`, then one
  character per tile: `.` empty, `#` ground, `=` platform, `S` start,
  `G` goal), wider than the window.
- A player that walks, jumps, and falls under gravity, colliding
  correctly against the tile grid (AABB, one axis resolved at a time).
- A camera that follows the player and scrolls the view, clamped so it
  never shows past the edges of the map.
- Sprite animation: idle, a two-frame walk cycle, and a jump frame,
  facing whichever direction the player last moved.

Source is split by concern, one file per module:

| File | Contents |
| --- | --- |
| `main.c` | SDL2 init, the game loop (event → update → render), cleanup |
| `map.c` / `map.h` | load a tile map, tile queries — no SDL2 anywhere |
| `player.c` / `player.h` | physics, collision resolution, animation state — no SDL2 function calls |
| `camera.c` / `camera.h` | follow + clamp + the world-to-screen transform |
| `render.c` / `render.h` | the only file that calls actual SDL2 drawing functions |

`map.c`, `player.c`, and `camera.c` never call an SDL2 function — only
compile-time constants/types from `<SDL2/SDL.h>` where needed — so
they link into a test binary with no SDL2 library at all.

Build and test your own version first. Use `solution/` to compare
once you are done, not before.

---

## Building the solution

```bash
cd solution
make -C libtci re
bash gen_assets.sh
make re
./platformer level1.txt
```

Controls: Left/Right arrows to move, Space to jump, Escape or closing
the window to quit.

`gen_assets.sh` needs Python3 + Pillow:

```bash
sudo apt install python3-pil
```

---

## What the tester checks

**Build** — the real game compiles and links with zero warnings.

**A standalone logic tester** — `map.o`, `player.o`, and `camera.o`
compiled and linked with `libtci.a` alone, no SDL2 at all, asserting
real outcomes against `fixtures/test-map.txt`:

- Falling from above onto open ground lands exactly on the ground
  tile, `on_ground` true.
- Falling onto a floating platform lands exactly on the platform tile,
  `on_ground` true.
- Walking into a two-tile wall stops exactly at its edge, still
  grounded.
- The camera clamps at both the left and right edges of the map.

**`platformer`** — runs its event loop for two seconds under a
headless (`SDL_VIDEODRIVER=dummy`) video driver without crashing. A
smoke test, not a visual check — actually playing the level is done by
running it yourself.

---

## License

MIT License. See [LICENSE](LICENSE).

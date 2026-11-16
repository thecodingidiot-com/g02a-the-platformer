# You're in

Everything is installed and built: the compiler, SDL2, valgrind, Pillow,
`libtci`, and the game's art. The terminal below is in this repository's
root. No terminal? *Terminal → New Terminal*.

## 1. Open the game's window

Open the **Ports** panel, next to the Terminal. Port 6080, *Game window*:
click the globe beside it. A desktop opens in a new browser tab, and every
window the game opens shows up there. Keep that tab.

## 2. Go back to the chapter

Setup's next step is a folder of your own, where you write the chapter:

```bash
mkdir mine
cp -rL solution/libtci solution/gen_assets.sh mine/
cd mine
make -C libtci re
```

Every page after that works in `mine/`. The chapter:
https://thecodingidiot.com/en-GB/chapters/g02a-the-platformer/01-setup

Starting on a later page? Copy the end of the page before yours instead of
making an empty `mine/`, e.g. `cp -rL steps/04-sprites-and-input mine` to
begin on page 05. The README's *Start at any page* lists them all.

## Or just play the finished game first

```bash
(cd solution && make re && ./platformer ../fixtures/level1.txt)
```

Arrows to move, Space to jump, Escape to quit. It plays in the desktop tab.

#!/bin/bash
# g02a — The Platformer / test.sh
#
# Builds the game, then checks the physics/collision/camera logic
# deterministically -- compiled and linked WITHOUT SDL2 at all (map.c,
# player.c, and camera.c never call an actual SDL function, only use
# compile-time constants/types from <SDL2/SDL.h>, so the logic layer
# needs no display and no SDL2 library at link time).
#
# Copy this file and fixtures/test-map.txt into your working directory,
# build with 'make re', then run:
#
#   bash test.sh

set -o pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
FIXTURES="${SCRIPT_DIR}/fixtures"

# ── colour ────────────────────────────────────────────────────────────────────

if [[ ! -t 1 ]]; then
    C_GREEN=""
    C_RED=""
    C_BOLD=""
    C_RESET=""
else
    C_GREEN="\033[0;32m"
    C_RED="\033[0;31m"
    C_BOLD="\033[1m"
    C_RESET="\033[0m"
fi

# ── state ─────────────────────────────────────────────────────────────────────

pass_count=0
fail_count=0
WORK_DIR=$(mktemp -d)

cleanup() {
    rm -rf "$WORK_DIR"
}
trap cleanup EXIT

# ── helpers ───────────────────────────────────────────────────────────────────

hr() {
    echo "────────────────────────────────────────────────────────────────"
}

banner() {
    hr
    echo "  g02a — The Platformer / test.sh"
    hr
}

pass() {
    local label="$1"
    printf "  ${C_GREEN}PASS${C_RESET}  %s\n" "$label"
    pass_count=$((pass_count + 1))
}

fail() {
    local label="$1"
    local detail="${2:-}"
    printf "  ${C_RED}FAIL${C_RESET}  %s\n" "$label"
    if [[ -n "$detail" ]]; then
        echo "        $detail"
    fi
    fail_count=$((fail_count + 1))
}

banner

# ── build the real game ───────────────────────────────────────────────────────

echo "Building..."
build_log=$(make re 2>&1)
build_status=$?
if [[ "$build_status" -ne 0 ]]; then
    fail "build succeeds" "make re failed:"
    echo "$build_log"
    exit 1
fi
pass "build succeeds"

if echo "$build_log" | grep -qi "warning"; then
    fail "build produces no warnings" "$(echo "$build_log" | grep -i warning)"
else
    pass "build produces no warnings"
fi

if [[ -x ./platformer ]]; then
    pass "platformer binary exists"
else
    fail "platformer binary exists"
fi

# ── build the SDL2-free logic tester ─────────────────────────────────────────

if [[ ! -f "${FIXTURES}/test-map.txt" ]]; then
    fail "fixtures/test-map.txt found" "keep the g02a-the-platformer clone alongside your working directory"
    exit 1
fi
cp "${FIXTURES}/test-map.txt" "$WORK_DIR/test-map.txt"

cat > "$WORK_DIR/test_logic.c" <<'TESTC'
#include <stdio.h>
#include "map.h"
#include "player.h"
#include "camera.h"

static int  g_pass = 0;
static int  g_fail = 0;

static void check_int(char const *label, int got, int want)
{
    if (got == want)
    {
        printf("PASS  %s (got %d)\n", label, got);
        g_pass++;
    }
    else
    {
        printf("FAIL  %s (got %d, want %d)\n", label, got, want);
        g_fail++;
    }
}

int main(void)
{
    t_map       map;
    t_player    p;
    t_camera    cam;
    int         i;

    if (!map_load(&map, "test-map.txt"))
    {
        printf("FAIL  map_load\n");
        return (1);
    }
    check_int("map width", map.width, 40);
    check_int("map height", map.height, 6);

    player_init(&p, 32.0f, 0.0f);
    i = 0;
    while (i < 100)
    {
        player_update_physics(&p);
        player_resolve_collision(&p, &map);
        i++;
    }
    check_int("free fall lands on open ground (on_ground)", p.on_ground, 1);
    check_int("free fall lands on open ground (y)", (int)p.y, 116);

    player_init(&p, 512.0f, 0.0f);
    i = 0;
    while (i < 100)
    {
        player_update_physics(&p);
        player_resolve_collision(&p, &map);
        i++;
    }
    check_int("free fall lands on the platform (on_ground)", p.on_ground, 1);
    check_int("free fall lands on the platform (y)", (int)p.y, 20);

    player_init(&p, 32.0f, 0.0f);
    i = 0;
    while (i < 100)
    {
        player_update_physics(&p);
        player_resolve_collision(&p, &map);
        i++;
    }
    i = 0;
    while (i < 100)
    {
        p.vx = MOVE_SPEED;
        player_update_physics(&p);
        player_resolve_collision(&p, &map);
        i++;
    }
    check_int("walking right stops at the wall (x)", (int)p.x, 164);
    check_int("walking right stops at the wall (still grounded)", p.on_ground, 1);

    camera_init(&cam);
    camera_update(&cam, 0.0f, &map);
    check_int("camera clamps at the left map edge", cam.x, 0);
    camera_update(&cam, 1280.0f, &map);
    check_int("camera clamps at the right map edge", cam.x, 480);

    map_free(&map);
    printf("\n%d passed, %d failed\n", g_pass, g_fail);
    return (g_fail > 0);
}
TESTC

logic_build_log=$(gcc -Wall -Wextra -I libtci -I . -c "$WORK_DIR/test_logic.c" -o "$WORK_DIR/test_logic.o" 2>&1 \
    && gcc "$WORK_DIR/test_logic.o" map.o player.o camera.o libtci/libtci.a -o "$WORK_DIR/test_logic" 2>&1)
logic_build_status=$?

if [[ "$logic_build_status" -ne 0 ]]; then
    fail "logic tester builds without SDL2" "$logic_build_log"
    exit 1
fi
pass "logic tester builds without SDL2 (map.o/player.o/camera.o only)"

echo
echo "Running the logic tester..."
cd "$WORK_DIR"
logic_out=$(./test_logic)
logic_status=$?
cd - > /dev/null

echo "$logic_out" | grep "^PASS\|^FAIL" | while read -r line; do
    echo "  $line"
done

logic_pass_count=$(echo "$logic_out" | grep -c "^PASS")
logic_fail_count=$(echo "$logic_out" | grep -c "^FAIL")
pass_count=$((pass_count + logic_pass_count))
fail_count=$((fail_count + logic_fail_count))

if [[ "$logic_status" -ne 0 ]]; then
    fail "all logic assertions pass" "see failures above"
fi

# ── headless smoke test of the real binary ───────────────────────────────────

echo
echo "Running platformer headless (2s)..."
SDL_VIDEODRIVER=dummy timeout 2 ./platformer level1.txt
platformer_status=$?
if [[ "$platformer_status" -eq 124 ]]; then
    pass "platformer runs its event loop for 2s without crashing"
else
    fail "platformer runs its event loop for 2s without crashing" "exit code: $platformer_status"
fi

# ── summary ───────────────────────────────────────────────────────────────────

echo
hr
printf "  ${C_BOLD}%d passed, %d failed${C_RESET}\n" "$pass_count" "$fail_count"
hr

if [[ "$fail_count" -gt 0 ]]; then
    exit 1
fi
exit 0

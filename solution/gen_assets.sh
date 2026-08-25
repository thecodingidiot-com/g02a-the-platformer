#!/bin/bash
# Generate the tileset and sprite sheet for g02a.
#
# tileset.png: 2 tiles, 32x32 each, side by side (64x32 total).
#   index 0 (src.x=0)  -- ground (brown)
#   index 1 (src.x=32) -- platform (green)
#
# spritesheet.png: 4 frames, 28x44 each, side by side (112x44 total).
#   frame 0 -- idle
#   frame 1, 2 -- walk cycle
#   frame 3 -- jump

set -e

mkdir -p assets

python3 - <<'EOF'
from PIL import Image, ImageDraw

TILE = 32
tileset = Image.new("RGB", (TILE * 2, TILE), (0, 0, 0))
d = ImageDraw.Draw(tileset)
d.rectangle([0, 0, TILE - 1, TILE - 1], fill=(0x8b, 0x5a, 0x2b))       # ground
d.rectangle([TILE, 0, TILE * 2 - 1, TILE - 1], fill=(0x2e, 0x8b, 0x57))  # platform
tileset.save("assets/tileset.png")
print("  wrote assets/tileset.png")

FW, FH = 28, 44
sheet = Image.new("RGBA", (FW * 4, FH), (0, 0, 0, 0))
d = ImageDraw.Draw(sheet)

def draw_frame(i, leg_a_top, leg_b_top):
    x0 = i * FW
    d.ellipse([x0 + 8, 0, x0 + FW - 9, 10], fill=(0xf0, 0xc8, 0xa0))    # head
    d.rectangle([x0 + 6, 10, x0 + FW - 7, 26], fill=(0xd4, 0x33, 0x33)) # torso
    d.rectangle([x0 + 8, leg_a_top, x0 + 13, 40], fill=(0x2b, 0x2b, 0x2b))
    d.rectangle([x0 + FW - 14, leg_b_top, x0 + FW - 9, 40], fill=(0x2b, 0x2b, 0x2b))

draw_frame(0, 26, 26)   # idle -- both legs level
draw_frame(1, 26, 32)   # walk A -- left leg forward
draw_frame(2, 32, 26)   # walk B -- right leg forward
draw_frame(3, 30, 30)   # jump -- legs tucked up together

sheet.save("assets/spritesheet.png")
print("  wrote assets/spritesheet.png")
EOF

#!/bin/sh
# Start the browser desktop once: an X display :1 (no password -- the port is only
# reachable through your own forward) and noVNC serving it on port 6080.
# Open http://localhost:6080/vnc.html?autoconnect=1&resize=scale, then run the game.
pgrep -x Xtigervnc >/dev/null 2>&1 && exit 0
Xtigervnc :1 -geometry 1024x768 -depth 24 -SecurityTypes None -localhost yes >/tmp/vnc.log 2>&1 &
websockify --web /usr/share/novnc 6080 localhost:5901 >/tmp/novnc.log 2>&1 &

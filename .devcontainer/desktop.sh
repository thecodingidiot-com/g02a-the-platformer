#!/bin/sh
# Start the browser desktop: an X display :1 (no password -- the port is only
# reachable through your own forward) and noVNC serving it on port 6080.
# Open http://localhost:6080 (Codespaces: the globe beside 6080 in Ports), then
# run the game.
#
# setsid -f puts each one in a session of its own: Codespaces and VS Code kill
# whatever a postStartCommand leaves running in its own process group when the
# command returns, which left port 6080 answering 502. Safe to run again: it
# starts only what isn't running.
pgrep -x Xtigervnc >/dev/null 2>&1 ||
    setsid -f Xtigervnc :1 -geometry 1024x768 -depth 24 -SecurityTypes None -localhost yes \
        </dev/null >/tmp/vnc.log 2>&1
pgrep -f 'websockify .*6080' >/dev/null 2>&1 ||
    setsid -f websockify --web /usr/share/novnc 6080 localhost:5901 \
        </dev/null >/tmp/novnc.log 2>&1
exit 0

#!/bin/sh
geany -i src/* make/* levels/* gfx/*.py dev.sh &
arduino src/src.ino &
gimp -n gfx/*.png &
cd make

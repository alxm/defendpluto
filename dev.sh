#!/bin/sh
geany -i src/* make/* levels/* gfx/*.py dev.sh README.md sketchsize &
arduino src/src.ino &
gimp -n gfx/*.png &
cd make

#!/bin/sh
geany -i DefendPluto/* make/* assets/levels/* assets/gfx/*.py assets/sfx/*.py dev.sh README.md &
arduino DefendPluto/DefendPluto.ino &
gimp -n assets/gfx/*.png assets/gfx/*.BMP &
cd make

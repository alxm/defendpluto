#!/bin/sh
geany -i DefendPluto/* make/* assets/levels/* assets/gfx/*.py dev.sh README.md &
arduino DefendPluto/DefendPluto.ino &
gimp -n assets/gfx/*.png &
cd make

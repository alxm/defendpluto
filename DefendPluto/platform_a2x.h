/*
    Copyright 2017-2018 Alex Margarit <alex@alxm.org>
    This file is part of Defend Pluto, a video game.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <a2x.h>

typedef APixel ZPixel;

#include "util_graphics.h"
#include "util_sound.h"

#define Z_SCREEN_W 80
#define Z_SCREEN_H 64

#define z_sprite_load(Index, Id) \
    z_platform__loadSprite(Index, "assets/gfx/" #Id ".png");

#define z_sfx_load(Index, Id) \
    z_platform__loadSfx(Index, "assets/sfx/" #Id ".wav");

extern void z_platform__loadSprite(ZSpriteId Sprite, const char* Path);
extern void z_platform__loadSfx(ZSfxId Sfx, const char* Path);

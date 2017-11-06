/*
    Copyright 2017 Alex Margarit <alex@alxm.org>

    arduboy-shooter is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    arduboy-shooter is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with arduboy-shooter.  If not, see <http://www.gnu.org/licenses/>.
*/

typedef enum {
    Z_BIT_RESTING = 0,
    Z_BIT_FORWARD = (1 << 0),
    Z_BIT_BACK    = (1 << 1),
    Z_BIT_LEFT    = (1 << 2),
    Z_BIT_RIGHT   = (1 << 3),
    Z_BIT_MAX_VAL = (Z_BIT_BACK | Z_BIT_RIGHT)
} ZFrameBits;

typedef struct {
    ZSprite enemy[3];
    ZSprite player[Z_BIT_MAX_VAL + 1];
    ZSprite hearts;
} ZGraphics;

extern ZGraphics z_graphics;

extern void z_graphics_setup(void);

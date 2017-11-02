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

#include "platform.h"

static uint8_t g_shakeFrames;
int8_t z_screen_xShake, z_screen_yShake;

void z_screen_tick(void)
{
    if(g_shakeFrames) {
        g_shakeFrames--;
        z_screen_xShake = (int8_t)((-1 + z_random_int8(3)) * 2);
        z_screen_yShake = (int8_t)((-1 + z_random_int8(3)) * 2);
    } else {
        z_screen_xShake = 0;
        z_screen_yShake = 0;
    }
}

void z_screen_shake(uint8_t Frames)
{
    g_shakeFrames = Frames;
}

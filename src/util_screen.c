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
#include "util_random.h"
#include "util_screen.h"

ZScreen z_screen;
static uint8_t g_shakeFrames;

void z_screen_reset(void)
{
    z_screen.xShake = 0;
    z_screen.yShake = 0;

    g_shakeFrames = 0;
}

void z_screen_tick(void)
{
    if(g_shakeFrames) {
        g_shakeFrames--;
        z_screen.xShake = u4(z_random_uint8(3));
        z_screen.yShake = u4(z_random_uint8(3));
    } else {
        z_screen.xShake = 1;
        z_screen.yShake = 1;
    }
}

void z_screen_shake(uint8_t Frames)
{
    g_shakeFrames = Frames;
}

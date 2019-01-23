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

#include "platform.h"
#include "util_screen.h"

#include "util_fix.h"
#include "util_input.h"
#include "util_timer.h"

ZScreen z_screen;
static uint8_t g_shakeFrames;

void z_screen_reset(void)
{
    z_screen.xShake = 1;
    z_screen.yShake = 1;

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

void z_screen_shake(uint8_t Ds)
{
    g_shakeFrames = z_timer_dsToTicks(Ds);
}

bool z_screen_tickPressA(void)
{
    bool pressed = z_button_pressedOnce(Z_BUTTON_A);

    if(pressed) {
        z_sfx_play(Z_SFX_PRESSED_A);
    }

    return pressed;
}

void z_screen_drawPressA(int16_t X, int16_t Y, ZFontId Font, ZFontAlign Align)
{
    if(z_fps_getCounter() & 0x30) {
        z_font_text(Z_STR_PRESS_A, X, Y, Font, Align);
    }
}

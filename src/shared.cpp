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

#include <Arduino.h>
#include <Arduboy2.h>
#include "shared.h"

extern Arduboy2Base g_arduboy;
ZControls z_controls;

void z_shared_setup(void)
{
    z_controls.up = UP_BUTTON;
    z_controls.down = DOWN_BUTTON;
    z_controls.left = LEFT_BUTTON;
    z_controls.right = RIGHT_BUTTON;
    z_controls.a = A_BUTTON;
    z_controls.b = B_BUTTON;
}

unsigned z_fps_getCounter(void)
{
    return g_arduboy.frameCount;
}

bool z_fps_isNthFrame(unsigned N)
{
    return g_arduboy.everyXFrames(N);
}

bool z_button_pressed(ZButton Button)
{
    return g_arduboy.pressed(Button);
}

void z_draw_fill(bool White)
{
    g_arduboy.fillScreen(White ? WHITE : BLACK);
}

void z_draw_rectangle(int X, int Y, int W, int H, bool White)
{
    g_arduboy.fillRect(X, Y, W, H, White ? WHITE : BLACK);
}

void z_draw_pixel(int X, int Y, bool White)
{
    g_arduboy.drawPixel(X, Y, White ? WHITE : BLACK);
}

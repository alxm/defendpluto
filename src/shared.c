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

#ifndef ARDUINO

#include "shared.h"

ZControls z_controls;

void z_shared_setup(void)
{
    z_controls.up = a_button_new("key.up gamepad.b.up");
    z_controls.down = a_button_new("key.down gamepad.b.down");
    z_controls.left = a_button_new("key.left gamepad.b.left");
    z_controls.right = a_button_new("key.right gamepad.b.right");
    z_controls.a = a_button_new("key.z gamepad.b.a");
    z_controls.b = a_button_new("key.x gamepad.b.b");
}

unsigned s_fps_getCounter(void)
{
    return a_fps_getCounter();
}

bool s_fps_isNthFrame(unsigned N)
{
    return a_fps_isNthFrame(N);
}

bool s_button_pressed(SButton Button)
{
    return a_button_getPressed(Button);
}

void s_draw_fill(bool White)
{
    a_pixel_setHex(White ? 0xffffff : 0);
    a_draw_fill();
}

void s_draw_rectangle(int X, int Y, int W, int H, bool White)
{
    a_pixel_setHex(White ? 0xffffff : 0);
    a_draw_rectangle(X, Y, W, H);
}

void s_draw_pixel(int X, int Y, bool White)
{
    a_pixel_setHex(White ? 0xffffff : 0);
    a_draw_pixel(X, Y);
}

#endif // ifndef ARDUINO

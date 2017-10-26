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

#include <a2x.h>
#include "shared.h"

SButton s_buttons[S_BUTTON_NUM];

void s_setup(void)
{
    s_buttons[S_BUTTON_UP] = a_button_new("key.up");
    s_buttons[S_BUTTON_DOWN] = a_button_new("key.down");
    s_buttons[S_BUTTON_LEFT] = a_button_new("key.left");
    s_buttons[S_BUTTON_RIGHT] = a_button_new("key.right");
    s_buttons[S_BUTTON_A] = a_button_new("key.z gamepad.b.a");
    s_buttons[S_BUTTON_B] = a_button_new("key.x gamepad.b.b");
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

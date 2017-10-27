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
SButton s_buttons[S_BUTTON_NUM];

void s_setup(void)
{
    s_buttons[S_BUTTON_UP] = UP_BUTTON;
    s_buttons[S_BUTTON_DOWN] = DOWN_BUTTON;
    s_buttons[S_BUTTON_LEFT] = LEFT_BUTTON;
    s_buttons[S_BUTTON_RIGHT] = RIGHT_BUTTON;
    s_buttons[S_BUTTON_A] = A_BUTTON;
    s_buttons[S_BUTTON_B] = B_BUTTON;
}

bool s_fps_isNthFrame(unsigned N)
{
    return g_arduboy.everyXFrames(N);
}

bool s_button_pressed(SButton Button)
{
    return g_arduboy.pressed(Button);
}

void s_draw_fill(bool White)
{
    g_arduboy.fillScreen(White ? WHITE : BLACK);
}

void s_draw_rectangle(int X, int Y, int W, int H, bool White)
{
    g_arduboy.fillRect(X, Y, W, H, White ? WHITE : BLACK);
}

void s_draw_pixel(int X, int Y, bool White)
{
    g_arduboy.drawPixel(X, Y, White ? WHITE : BLACK);
}

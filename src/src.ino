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
#include "loop.h"

Arduboy2Base g_arduboy;

void setup()
{
    g_arduboy.begin();
    g_arduboy.setFrameRate(S_FPS);

    s_setup();
    loop_setup();
}

void loop()
{
    if(!g_arduboy.nextFrameDEV()) {
        g_arduboy.idle();
        return;
    }

    g_arduboy.pollButtons();

    loop_tick();
    loop_draw();

    g_arduboy.display();
}

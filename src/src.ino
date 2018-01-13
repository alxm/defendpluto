/*
    Copyright 2017, 2018 Alex Margarit <alex@alxm.org>

    Defend Pluto is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Defend Pluto is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Defend Pluto.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "platform.h"

#if Z_PLATFORM_ARDUBOY
    #include <Arduboy2.h>
#elif Z_PLATFORM_GAMEBUINOMETA
    #include <Gamebuino-Meta.h>
#endif

#include "loop.h"
#include "util_fps.h"

#if Z_PLATFORM_ARDUBOY
    Arduboy2Base g_arduboy;
#endif

void setup()
{
    #if Z_PLATFORM_ARDUBOY
        g_arduboy.begin();
        g_arduboy.setFrameRate(Z_FPS_RATE);
    #elif Z_PLATFORM_GAMEBUINOMETA
        gb.begin();
        gb.setFrameRate(Z_FPS_RATE);
    #endif

    z_loop_setup();
}

void loop()
{
    #if Z_PLATFORM_ARDUBOY
        #if Z_DEBUG_STATS
            #define Z_NEXT_FRAME nextFrameDEV
        #else
            #define Z_NEXT_FRAME nextFrame
        #endif

        if(!g_arduboy.Z_NEXT_FRAME()) {
            return;
        }

        g_arduboy.pollButtons();
    #elif Z_PLATFORM_GAMEBUINOMETA
        if(!gb.update()) {
            return;
        }
    #endif

    z_loop_tick();
    z_loop_draw();

    #if Z_PLATFORM_ARDUBOY
        g_arduboy.display();
    #endif
}

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
#include "loop.h"

#define STRINGIFY2(X) #X
#define STRINGIFY(X) STRINGIFY2(X)

A_SETUP
{
    a_settings_set("app.title", "arduboy-shooter");
    a_settings_set("app.version", "1.0");
    a_settings_set("app.author", "alxm");
    a_settings_set("app.output.on", "yes");
    a_settings_set("video.width", STRINGIFY(S_WIDTH));
    a_settings_set("video.height", STRINGIFY(S_HEIGHT));
    a_settings_set("video.fps", STRINGIFY(S_FPS));
}

A_STATE(run)
{
    A_STATE_INIT
    {
        loop_setup();
    }

    A_STATE_TICK
    {
        loop_tick();
    }

    A_STATE_DRAW
    {
        loop_draw();
    }
}

A_MAIN
{
    s_setup();

    a_state_new("run", run, "", "");
    a_state_push("run");
}

#endif // ifndef ARDUINO

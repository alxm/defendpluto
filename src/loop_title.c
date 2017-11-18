/*
    Copyright 2017 Alex Margarit <alex@alxm.org>

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
#include "loop.h"
#include "util_font.h"
#include "util_graphics.h"
#include "util_input.h"

void z_loop_title_init(void)
{
    //
}

void z_loop_title_free(void)
{
    //
}

void z_loop_title_tick(void)
{
    if(z_button_pressed(z_controls.a)) {
        z_loop_setState(Z_STATE_GAME);
    }
}

void z_loop_title_draw(void)
{
    z_draw_fill(Z_COLOR_BLUE);
    z_font_text("Press FIRE to Start", 8, 26, Z_FONT_FACE_ALPHANUM);
}

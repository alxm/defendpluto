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
#include "state_over.h"

#include "obj_star.h"
#include "util_font.h"
#include "util_input.h"
#include "util_pool.h"
#include "util_screen.h"
#include "util_str.h"

void z_state_over_init(void)
{
    z_sfx_play(Z_SFX_LEVEL_LOST);
}

void z_state_over_tick(void)
{
    z_pool_tick(Z_POOL_STAR, z_star_tick, NULL);

    if(z_state_changed()) {
        return;
    }

    if(z_screen_tickPressA()) {
        z_state_set(Z_STATE_TITLE, true);
    }
}

void z_state_over_draw(void)
{
    z_draw_fill(Z_COLOR_BLUE);
    z_pool_draw(Z_POOL_STAR, z_star_draw);
    z_font_textWrap(Z_STR_END, 4, 4, Z_FONT_FACE_RED);
    z_screen_drawPressA(4, 52, Z_FONT_FACE_YELLOW, Z_FONT_ALIGN_L);
}

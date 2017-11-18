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
#include "util_fps.h"
#include "util_graphics.h"
#include "util_input.h"
#include "util_pool.h"
#include "util_screen.h"
#include "obj_star.h"

static bool g_blink = false;

void z_loop_title_init(void)
{
    g_blink = true;
}

void z_loop_title_free(void)
{
    //
}

void z_loop_title_tick(void)
{
    z_pool_tick(Z_POOL_STAR, z_star_tick);
    z_star_spawn();

    if(z_fps_isNthFrame(Z_FPS * 3 / 2)) {
        g_blink = !g_blink;
    }

    if(z_button_pressed(z_controls.a)) {
        z_loop_setState(Z_STATE_GAME);
    }
}

void z_loop_title_draw(void)
{
    z_draw_fill(Z_COLOR_BLUE);
    z_pool_draw(Z_POOL_STAR, z_star_draw);
    z_sprite_blitCentered(&z_graphics.title, Z_WIDTH / 2, Z_HEIGHT / 2, 0);

    if(g_blink) {
        z_font_text("Press FIRE to Start", 8, 39, Z_FONT_FACE_ALPHANUM);
    }
}
/*
    Copyright 2018 Alex Margarit <alex@alxm.org>

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
#include "util_fix.h"
#include "util_graphics.h"
#include "util_input.h"
#include "util_pool.h"
#include "util_screen.h"
#include "util_str.h"
#include "obj_star.h"

static uint8_t g_state;

void z_loop_win_init(void)
{
    g_state = 0;
}

void z_loop_win_tick(void)
{
    z_pool_tick(Z_POOL_STAR, z_star_tick, NULL);

    if(z_button_pressedOnce(Z_BUTTON_A)) {
        if(g_state++ > 0) {
            z_loop_setState(Z_STATE_SWIPE_HIDE);
        }
    }
}

void z_loop_win_draw(void)
{
    z_draw_fill(Z_COLOR_BLUE);
    z_pool_draw(Z_POOL_STAR, z_star_draw);

    z_font_textWrap(g_state == 0
                        ? Z_STR_WIN
                        : Z_STR_THANKS,
                    4,
                    4,
                    Z_FONT_FACE_YELLOW);

    z_screen_drawPressA(4, 52, Z_FONT_FACE_REDO, Z_FONT_ALIGN_L);
}

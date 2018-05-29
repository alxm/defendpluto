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
#include "state_title.h"

#include "obj_player.h"
#include "obj_star.h"
#include "util_input.h"
#include "util_font.h"
#include "util_pool.h"
#include "util_screen.h"

void z_state_title_init(void)
{
    z_player_resetPosition();
    z_sfx_play(Z_SFX_TITLE_SCREEN);
}

void z_state_title_tick(void)
{
    z_pool_tick(Z_POOL_STAR, z_star_tick, NULL);

    if(z_state_changed()) {
        return;
    }

    if(z_screen_tickPressA()) {
        z_state_set(Z_STATE_NEW, true);
    }
}

void z_state_title_draw(void)
{
    z_draw_fill(Z_COLOR_BLUE);
    z_pool_draw(Z_POOL_STAR, z_star_draw);
    z_sprite_blit(Z_SPRITE_TITLE, 0, 13, 0);

    int y = 13 + z_sprite_getHeight(Z_SPRITE_TITLE);
    z_draw_rectangle(0, i16(y), Z_SCREEN_W, i16(Z_SCREEN_H - y), Z_COLOR_BLUE);

    z_sprite_blit(Z_SPRITE_ALXM_WING,
                  i16(Z_SCREEN_W - 1 - z_sprite_getWidth(Z_SPRITE_ALXM_WING)),
                  2,
                  0);

    z_screen_drawPressA(Z_SCREEN_W / 2, 49, Z_FONT_FACE_REDO, Z_FONT_ALIGN_C);
}

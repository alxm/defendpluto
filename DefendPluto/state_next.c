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
#include "state_next.h"

#include "obj_player.h"
#include "obj_star.h"
#include "util_font.h"
#include "util_hud.h"
#include "util_input.h"
#include "util_pool.h"
#include "util_screen.h"
#include "util_str.h"

void z_state_next_init(void)
{
    z_hud_reset();
    z_screen_reset();

    z_player_resetPosition();
    z_pool_clear(Z_POOL_BULLETP);

    z_sfx_play(Z_SFX_LEVEL_COMPLETE);
}

void z_state_next_tick(bool Active)
{
    z_pool_tick(Z_POOL_STAR, z_star_tick, NULL);

    if(!Active) {
        return;
    }

    if(z_button_pressedOnce(Z_BUTTON_A)) {
        z_sfx_play(Z_SFX_PRESSED_A);
        z_state_set(Z_STATE_PLAY, true);
        z_player_setLevel(u8(z_player_getLevel() + 1));
    }
}

void z_state_next_draw(void)
{
    z_draw_fill(Z_COLOR_BLUE);
    z_pool_draw(Z_POOL_STAR, z_star_draw);
    z_sprite_blitCentered(Z_SPRITE_DEFENDPLUTO, Z_SCREEN_W / 2, 12, 0);

    z_font_text(Z_STR_LEVEL_CLEARED_1,
                Z_SCREEN_W / 2,
                23,
                Z_FONT_FACE_YELLOWO,
                Z_FONT_ALIGN_C);

    z_font_text(Z_STR_LEVEL_CLEARED_2,
                Z_SCREEN_W / 2,
                32,
                Z_FONT_FACE_YELLOWO,
                Z_FONT_ALIGN_C);

    z_screen_drawPressA(Z_SCREEN_W / 2, 48, Z_FONT_FACE_REDO, Z_FONT_ALIGN_C);
}

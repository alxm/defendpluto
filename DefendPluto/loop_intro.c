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
#include "loop_intro.h"

#include "util_timer.h"

static uint8_t g_stage;
static int16_t g_height;

static const uint8_t Z_LOGO_WAIT_DS = 12;

void z_loop_intro_init(void)
{
    g_stage = 0;
    g_height = z_sprite_getHeight(Z_SPRITE_ALXM);

    z_draw_fill(Z_COLOR_ALXM_BG);
    z_timer_start(Z_TIMER_G1, 1);
}

void z_loop_intro_tick(void)
{
    if(z_timer_expired(Z_TIMER_G1)) {
        switch(g_stage) {
            case 0: {
                if(--g_height == 0) {
                    g_stage = 1;
                }
            } break;

            case 1: {
                if(++g_height == Z_LOGO_WAIT_DS) {
                    g_stage = 2;
                    g_height = 0;
                }
            } break;

            case 2: {
                if(++g_height > z_sprite_getHeight(Z_SPRITE_ALXM)) {
                    z_loop_setState(Z_STATE_SWIPE_INTRO);
                }
            } break;
        }
    }
}

void z_loop_intro_draw(void)
{
    int16_t spriteW = z_sprite_getWidth(Z_SPRITE_ALXM);
    int16_t spriteH = z_sprite_getHeight(Z_SPRITE_ALXM);

    z_sprite_blitCentered(Z_SPRITE_ALXM, Z_SCREEN_W / 2, Z_SCREEN_H / 2, 0);

    switch(g_stage) {
        case 0: {
            z_draw_rectangle(i16(Z_SCREEN_W / 2 - spriteW / 2),
                             i16(Z_SCREEN_H / 2 - spriteH / 2),
                             spriteW,
                             g_height,
                             Z_COLOR_ALXM_BG);
        } break;

        case 2: {
            z_draw_rectangle(i16(Z_SCREEN_W / 2 - spriteW / 2),
                             i16(Z_SCREEN_H / 2 + (spriteH + 1) / 2 - g_height),
                             spriteW,
                             g_height,
                             Z_COLOR_ALXM_BG);
        } break;
    }
}

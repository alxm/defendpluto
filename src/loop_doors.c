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
#include "util_fix.h"
#include "util_graphics.h"
#include "util_hud.h"
#include "util_pool.h"
#include "util_screen.h"
#include "obj_bulletp.h"
#include "obj_player.h"
#include "obj_star.h"

static const uint8_t Z_SLIDE_CLOSE_INC = 2;
static const uint8_t Z_SLIDE_OPEN_INC = 1;

static uint8_t g_height;

void z_loop_doors_close_init(void)
{
    g_height = 0;
}

void z_loop_doors_open_init(void)
{
    g_height = u8(Z_SCREEN_H / 2);
}

static void sharedTick(void)
{
    z_screen_tick();
    z_hud_tick();
    z_player_tick(false);
    z_pool_tick(Z_POOL_STAR, z_star_tick, NULL);
    z_pool_tick(Z_POOL_BULLETP, z_bulletp_tick, NULL);
}

void z_loop_doors_close_tick(void)
{
    sharedTick();

    g_height = u8(g_height + Z_SLIDE_CLOSE_INC);

    if(g_height > Z_SCREEN_H / 2) {
        switch(z_loop_getLastState()) {
            case Z_STATE_TITLE: {
                z_loop_setState(Z_STATE_NEW);
            } break;

            case Z_STATE_PLAY: {
                z_loop_setState(Z_STATE_NEXT);
            } break;

            case Z_STATE_DIED: {
                z_loop_setState(Z_STATE_OVER);
            } break;

            case Z_STATE_OVER: {
                z_loop_setState(Z_STATE_TITLE);
            } break;
        }
    }
}

void z_loop_doors_intro_tick(void)
{
    z_pool_tick(Z_POOL_STAR, z_star_tick, NULL);

    if(g_height < Z_SLIDE_OPEN_INC) {
        z_loop_setState(Z_STATE_TITLE);
    } else {
        g_height = u8(g_height - Z_SLIDE_OPEN_INC);
    }
}

void z_loop_doors_open_tick(void)
{
    sharedTick();

    if(g_height < Z_SLIDE_OPEN_INC) {
        z_loop_setState(Z_STATE_PLAY);
    } else {
        g_height = u8(g_height - Z_SLIDE_OPEN_INC);
    }
}

void z_loop_doors_draw(void)
{
    z_draw_fill(Z_COLOR_BLUE);
    z_pool_draw(Z_POOL_STAR, z_star_draw);

    switch(z_loop_getLastState()) {
        case Z_STATE_DIED:
        case Z_STATE_NEW:
        case Z_STATE_NEXT:
        case Z_STATE_PLAY: {
            z_pool_draw(Z_POOL_BULLETP, z_bulletp_draw);
            z_player_draw();
            z_hud_draw();
        } break;
    }

    z_draw_rectangle(0, 0, Z_SCREEN_W, i16(g_height - 1), Z_COLOR_BLUE);
    z_draw_hline(0, i16(Z_SCREEN_W - 1), i16(g_height - 1), Z_COLOR_YELLOW);

    z_draw_hline(0,
                 i16(Z_SCREEN_W - 1),
                 i16(Z_SCREEN_H - g_height),
                 Z_COLOR_YELLOW);
    z_draw_rectangle(0,
                     i16(Z_SCREEN_H - g_height + 1),
                     Z_SCREEN_W,
                     i16(g_height - 1),
                     Z_COLOR_BLUE);
}

void z_loop_doors_intro_draw(void)
{
    z_draw_fill(Z_COLOR_BLUE);
    z_pool_draw(Z_POOL_STAR, z_star_draw);
    z_sprite_blit(Z_SPRITE_TITLE, 0, 13, 0);
    z_draw_rectangle(0, 50, Z_SCREEN_W, 14, Z_COLOR_BLUE);

    z_draw_rectangle(0, 0, Z_SCREEN_W, i16(g_height - 1), Z_COLOR_ALXM_BG);
    z_draw_hline(0, i16(Z_SCREEN_W - 1), i16(g_height - 1), Z_COLOR_YELLOW);

    z_draw_hline(0,
                 i16(Z_SCREEN_W - 1),
                 i16(Z_SCREEN_H - g_height),
                 Z_COLOR_YELLOW);
    z_draw_rectangle(0,
                     i16(Z_SCREEN_H - g_height + 1),
                     Z_SCREEN_W,
                     i16(g_height - 1),
                     Z_COLOR_ALXM_BG);
}

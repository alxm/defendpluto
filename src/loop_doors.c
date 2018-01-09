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
#include "obj_player.h"
#include "obj_star.h"

#define Z_SLIDE_CLOSE_INC 2
#define Z_SLIDE_OPEN_INC  1

uint8_t g_height;

void z_loop_doors_close_init(void)
{
    g_height = Z_SLIDE_CLOSE_INC;
}

void z_loop_doors_open_init(void)
{
    g_height = Z_HEIGHT / 2 - Z_SLIDE_OPEN_INC;
}

static void sharedTick(void)
{
    z_screen_tick();
    z_hud_tick();
    z_player_tick(false);
    z_pool_tick(Z_POOL_STAR, z_star_tick, NULL);
}

void z_loop_doors_close_tick(void)
{
    sharedTick();

    g_height = u8(g_height + Z_SLIDE_CLOSE_INC);

    if(g_height > Z_HEIGHT / 2) {
        z_loop_setState(Z_STATE_NEXT);
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
    z_player_draw();
    z_hud_draw();

    #define Z_OVERLAP 3

    #define Z_SOLID Z_COLOR_LIGHTBLUE2
    #define Z_TRIM_1 Z_COLOR_LIGHTBLUE
    #define Z_TRIM_2 Z_COLOR_BLUE

    // Bottom
    z_draw_vline(0, i16(Z_HEIGHT - g_height), Z_HEIGHT - 1, Z_TRIM_1);
    z_draw_vline(1, i16(Z_HEIGHT - g_height + 3), Z_HEIGHT - 1, Z_TRIM_2);
    z_draw_vline(Z_WIDTH - 2, i16(Z_HEIGHT - g_height + 3), Z_HEIGHT - 1, Z_TRIM_2);
    z_draw_vline(Z_WIDTH - 1, i16(Z_HEIGHT - g_height), Z_HEIGHT - 1, Z_TRIM_1);

    z_draw_hline(1, Z_WIDTH - 2, i16(Z_HEIGHT - g_height), Z_TRIM_1);
    z_draw_hline(1, Z_WIDTH - 2, i16(Z_HEIGHT - g_height + 1), Z_TRIM_2);
    z_draw_hline(1, Z_WIDTH - 2, i16(Z_HEIGHT - g_height + 2), Z_TRIM_1);
    z_draw_hline(1, Z_WIDTH - 2, i16(Z_HEIGHT - g_height + 3), Z_TRIM_2);

    z_draw_rectangle(2, i16(Z_HEIGHT - g_height + 4), Z_WIDTH - 4, i8(g_height - 4), Z_SOLID);

    // Top
    z_draw_vline(0, 0, i16(g_height - 1 + Z_OVERLAP), Z_TRIM_1);
    z_draw_vline(1, 0, i16(g_height - 4 + Z_OVERLAP), Z_TRIM_2);
    z_draw_vline(Z_WIDTH - 2, 0, i16(g_height - 4 + Z_OVERLAP), Z_TRIM_2);
    z_draw_vline(Z_WIDTH - 1, 0, i16(g_height - 1 + Z_OVERLAP), Z_TRIM_1);

    z_draw_hline(1, Z_WIDTH - 2, i16(g_height - 4 + Z_OVERLAP), Z_TRIM_2);
    z_draw_hline(1, Z_WIDTH - 2, i16(g_height - 3 + Z_OVERLAP), Z_TRIM_1);
    z_draw_hline(1, Z_WIDTH - 2, i16(g_height - 2 + Z_OVERLAP), Z_TRIM_2);
    z_draw_hline(1, Z_WIDTH - 2, i16(g_height - 1 + Z_OVERLAP), Z_TRIM_1);

    z_draw_rectangle(2, 0, Z_WIDTH - 4, i8(g_height - 4 + Z_OVERLAP), Z_SOLID);
}

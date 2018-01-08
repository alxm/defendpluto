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
#include "util_fps.h"
#include "util_graphics.h"
#include "util_random.h"
#include "util_screen.h"

ZScreen z_screen;
static uint8_t g_shakeFrames;

static struct {
    bool active : 1;
    bool directionClosing : 1;
    uint8_t height : 6;
} g_doors;

#define Z_SLIDE_CLOSE_INC 2
#define Z_SLIDE_OPEN_INC  1

void z_screen_reset(void)
{
    z_screen.xShake = 1;
    z_screen.yShake = 1;

    g_shakeFrames = 0;
    g_doors.active = false;
}

void z_screen_tick(void)
{
    if(g_shakeFrames) {
        g_shakeFrames--;
        z_screen.xShake = u4(z_random_uint8(3));
        z_screen.yShake = u4(z_random_uint8(3));
    } else {
        z_screen.xShake = 1;
        z_screen.yShake = 1;
    }

    if(g_doors.active) {
        if(g_doors.directionClosing) {
            g_doors.height = uN(g_doors.height + Z_SLIDE_CLOSE_INC, 6);
            g_doors.active = g_doors.height != Z_HEIGHT / 2;
        } else {
            g_doors.height = uN(g_doors.height - Z_SLIDE_OPEN_INC, 6);
            g_doors.active = g_doors.height != 0;
        }
    }
}

void z_screen_draw(void)
{
    if(g_doors.height == 0) {
        return;
    }

    #define Z_OVERLAP 3

    #define Z_SOLID Z_COLOR_LIGHTBLUE2
    #define Z_TRIM_1 Z_COLOR_LIGHTBLUE
    #define Z_TRIM_2 Z_COLOR_BLUE

    // Bottom
    z_draw_vline(0, i16(Z_HEIGHT - g_doors.height), Z_HEIGHT - 1, Z_TRIM_1);
    z_draw_vline(1, i16(Z_HEIGHT - g_doors.height + 3), Z_HEIGHT - 1, Z_TRIM_2);
    z_draw_vline(Z_WIDTH - 2, i16(Z_HEIGHT - g_doors.height + 3), Z_HEIGHT - 1, Z_TRIM_2);
    z_draw_vline(Z_WIDTH - 1, i16(Z_HEIGHT - g_doors.height), Z_HEIGHT - 1, Z_TRIM_1);

    z_draw_hline(1, Z_WIDTH - 2, i16(Z_HEIGHT - g_doors.height), Z_TRIM_1);
    z_draw_hline(1, Z_WIDTH - 2, i16(Z_HEIGHT - g_doors.height + 1), Z_TRIM_2);
    z_draw_hline(1, Z_WIDTH - 2, i16(Z_HEIGHT - g_doors.height + 2), Z_TRIM_1);
    z_draw_hline(1, Z_WIDTH - 2, i16(Z_HEIGHT - g_doors.height + 3), Z_TRIM_2);

    z_draw_rectangle(2, i16(Z_HEIGHT - g_doors.height + 4), Z_WIDTH - 4, i8(g_doors.height - 4), Z_SOLID);

    // Top
    z_draw_vline(0, 0, i16(g_doors.height - 1 + Z_OVERLAP), Z_TRIM_1);
    z_draw_vline(1, 0, i16(g_doors.height - 4 + Z_OVERLAP), Z_TRIM_2);
    z_draw_vline(Z_WIDTH - 2, 0, i16(g_doors.height - 4 + Z_OVERLAP), Z_TRIM_2);
    z_draw_vline(Z_WIDTH - 1, 0, i16(g_doors.height - 1 + Z_OVERLAP), Z_TRIM_1);

    z_draw_hline(1, Z_WIDTH - 2, i16(g_doors.height - 4 + Z_OVERLAP), Z_TRIM_2);
    z_draw_hline(1, Z_WIDTH - 2, i16(g_doors.height - 3 + Z_OVERLAP), Z_TRIM_1);
    z_draw_hline(1, Z_WIDTH - 2, i16(g_doors.height - 2 + Z_OVERLAP), Z_TRIM_2);
    z_draw_hline(1, Z_WIDTH - 2, i16(g_doors.height - 1 + Z_OVERLAP), Z_TRIM_1);

    z_draw_rectangle(2, 0, Z_WIDTH - 4, i8(g_doors.height - 4 + Z_OVERLAP), Z_SOLID);
}

void z_screen_shake(uint8_t Ds)
{
    g_shakeFrames = Z_DS_TO_FRAMES(Ds);
}

bool z_screen_areDoorsMoving(void)
{
    return g_doors.active;
}

void z_screen_setDoors(bool Close)
{
    g_doors.active = true;
    g_doors.directionClosing = Close;

    if(Close) {
        g_doors.height = Z_SLIDE_CLOSE_INC;
    } else {
        g_doors.height = Z_HEIGHT / 2 - Z_SLIDE_OPEN_INC;
    }
}

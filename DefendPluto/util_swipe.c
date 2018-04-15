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
#include "util_swipe.h"

#include "util_fix.h"

typedef void (ZSwipeInit)(void);
typedef bool (ZSwipeTick)(void);
typedef void (ZSwipeDraw)(void);

#define Z_SLIDE_CLOSE_INC (2)
#define Z_SLIDE_OPEN_INC  (2)

static uint8_t g_counter;

static void swipeHideInit(void)
{
    g_counter = 0;
}

static bool swipeHideTick(void)
{
    g_counter = u8(g_counter + Z_SLIDE_CLOSE_INC);

    return g_counter == Z_ANGLE_090;
}

static void swipeShowInit(void)
{
    g_counter = Z_ANGLE_090;
}

static bool swipeShowTick(void)
{
    g_counter = u8(g_counter - Z_SLIDE_OPEN_INC);

    return g_counter == 0;
}

static void swipeDraw(void)
{
    int16_t h = z_fix_fixtoi(zf(z_fix_sin(g_counter) * (Z_SCREEN_H / 2)));

    z_draw_rectangle(0, 0, Z_SCREEN_W, h, Z_COLOR_BLUE);
    z_draw_hline(0, Z_SCREEN_W - 1, h, Z_COLOR_YELLOW);

    z_draw_hline(0, Z_SCREEN_W - 1, i16(Z_SCREEN_H - 1 - h), Z_COLOR_YELLOW);
    z_draw_rectangle(0, i16(Z_SCREEN_H - h), Z_SCREEN_W, h, Z_COLOR_BLUE);
}

static const struct {
    ZSwipeInit* init;
    ZSwipeTick* tick;
    ZSwipeDraw* draw;
} g_callbacks[Z_SWIPE_NUM] = {
    [Z_SWIPE_HIDE] = {swipeHideInit, swipeHideTick, swipeDraw},
    [Z_SWIPE_SHOW] = {swipeShowInit, swipeShowTick, swipeDraw},
};

void z_swipe_init(ZSwipeId Swipe)
{
    g_callbacks[Swipe].init();
}

bool z_swipe_tick(ZSwipeId Swipe)
{
    return g_callbacks[Swipe].tick();
}

void z_swipe_draw(ZSwipeId Swipe)
{
    g_callbacks[Swipe].draw();
}

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

#pragma once

#include "util_font.h"

typedef struct {
    uint8_t xShake : 4;
    uint8_t yShake : 4;
} ZScreen;

extern ZScreen z_screen;

extern void z_screen_reset(void);
extern void z_screen_tick(void);

extern void z_screen_shake(uint8_t Ds);

static inline int8_t z_screen_getXShake(void)
{
    return i8(-1 + z_screen.xShake);
}

static inline int8_t z_screen_getYShake(void)
{
    return i8(-1 + z_screen.yShake);
}

extern bool z_screen_tickPressA(void);
extern void z_screen_drawPressA(int16_t X, int16_t Y, ZFontId Font, ZFontAlign Align);

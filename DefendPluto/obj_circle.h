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

#pragma once

#include "util_fix.h"
#include "util_pool.h"

typedef struct {
    ZPoolObject poolObject;
    ZFix x, y;
    uint8_t radius;
} ZCircle;

extern void z_circle_init(ZCircle* Circle, ZFix X, ZFix Y);
extern ZPoolTickCallback z_circle_tick;
extern ZPoolDrawCallback z_circle_draw;

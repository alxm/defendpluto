/*
    Copyright 2017 Alex Margarit <alex@alxm.org>

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
    ZPoolObjHeader poolObject;
    ZFix x, y;
    uint8_t damage : 3;
} ZBulletP;

extern void z_bulletp_init(ZBulletP* Bullet, ZFix X, ZFix Y);
extern ZPoolTick z_bulletp_tick;
extern ZPoolDraw z_bulletp_draw;

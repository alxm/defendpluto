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
#include "obj_circle.h"

#include "util_effects.h"
#include "util_screen.h"

static const uint8_t Z_CIRCLE_RADIUS_MAX = 4;

void z_circle_init(ZCircle* Circle, ZFix X, ZFix Y)
{
    Circle->x = X;
    Circle->y = Y;
    Circle->radius = 0;
}

bool z_circle_tick(ZPoolObject* Circle, void* Context)
{
    Z_UNUSED(Context);

    ZCircle* circle = (ZCircle*)Circle;

    return circle->radius++ < Z_CIRCLE_RADIUS_MAX;
}

void z_circle_draw(ZPoolObject* Circle)
{
    ZCircle* circle = (ZCircle*)Circle;
    int16_t x = i16(z_fix_fixtoi(circle->x) + z_screen_getXShake());
    int16_t y = i16(z_fix_fixtoi(circle->y) + z_screen_getYShake());

    z_draw_circle(x, y, circle->radius, Z_COLOR_RED);
    z_draw_circle(x, y, u8(circle->radius * 2), Z_COLOR_RED);

    z_effect_light(circle->x, circle->y, Z_LIGHT_EXPLOSION);
}

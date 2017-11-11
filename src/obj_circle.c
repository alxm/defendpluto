/*
    Copyright 2017 Alex Margarit <alex@alxm.org>

    arduboy-shooter is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    arduboy-shooter is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with arduboy-shooter.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "platform.h"
#include "util_fix.h"
#include "util_graphics.h"
#include "util_pool.h"
#include "util_screen.h"
#include "obj_circle.h"

void z_circle_init(ZCircle* Circle, int8_t X, int8_t Y, uint8_t MaxRadius, ZFix GrowSpeed)
{
    Circle->radius = 0;
    Circle->growSpeed = GrowSpeed;
    Circle->x = X;
    Circle->y = Y;
    Circle->maxRadius = MaxRadius;
}

bool z_circle_tick(ZPoolObject* Circle)
{
    ZCircle* circle = (ZCircle*)Circle;

    circle->radius = zf(circle->radius + circle->growSpeed);

    return z_fix_fixtoi(circle->radius) <= circle->maxRadius;
}

void z_circle_draw(ZPoolObject* Circle)
{
    ZCircle* circle = (ZCircle*)Circle;

    z_draw_circle(i8(circle->x + z_screen_xShake),
                  i8(circle->y + z_screen_yShake),
                  u8(z_fix_fixtoi(circle->radius)),
                  Z_COLOR_RED);
}

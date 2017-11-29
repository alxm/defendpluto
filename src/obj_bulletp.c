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

#include "platform.h"
#include "util_collision.h"
#include "util_fix.h"
#include "util_graphics.h"
#include "util_pool.h"
#include "util_screen.h"
#include "obj_bulletp.h"
#include "obj_enemy.h"

void z_bulletp_init(ZBulletP* Bullet, ZFix X, ZFix Y)
{
    Bullet->x = X;
    Bullet->y = Y;
}

bool z_bulletp_tick(ZPoolObject* Bullet)
{
    ZBulletP* bullet = (ZBulletP*)Bullet;

    bullet->y = zf(bullet->y - z_fix_itofix(2));

    if(bullet->y < 0) {
        return false;
    }

    return !z_enemy_checkCollisions(z_fix_fixtoi(bullet->x),
                                    z_fix_fixtoi(bullet->y),
                                    2,
                                    4,
                                    false);
}

void z_bulletp_draw(ZPoolObject* Bullet)
{
    ZBulletP* bullet = (ZBulletP*)Bullet;

    int16_t x = z_fix_fixtoi(bullet->x);
    int16_t y = z_fix_fixtoi(bullet->y);

    z_draw_rectangle(i16(x - 1 + z_screen_getXShake()),
                     i16(y - 2 + z_screen_getYShake()),
                     2,
                     4,
                     Z_COLOR_RED);
}

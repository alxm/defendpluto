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
#include "util_pool.h"
#include "util_screen.h"
#include "obj_bullet.h"
#include "obj_enemy.h"

void z_bullet_init(ZBullet* Bullet, ZFix X, ZFix Y, ZFix Dy)
{
    Bullet->x = X;
    Bullet->y = Y;
    Bullet->dy = Dy;
}

bool z_bullet_tick(ZPoolObject* Bullet)
{
    ZBullet* bullet = (ZBullet*)Bullet;

    bullet->y = (ZFix)(bullet->y + bullet->dy);

    if((bullet->dy < 0 && bullet->y < 0)
        || (bullet->dy > 0 && z_fix_fixtoi(bullet->y) >= Z_HEIGHT)) {

        return false;
    }

    return !z_enemy_checkCollisions(z_fix_fixtoi(bullet->x),
                                    z_fix_fixtoi(bullet->y),
                                    2,
                                    4,
                                    false);
}

void z_bullet_draw(ZPoolObject* Bullet)
{
    ZBullet* bullet = (ZBullet*)Bullet;

    int8_t x = z_fix_fixtoi(bullet->x);
    int8_t y = z_fix_fixtoi(bullet->y);

    z_draw_rectangle((int8_t)(x - 1 + z_screen_xShake),
                     (int8_t)(y - 2 + z_screen_yShake),
                     2,
                     4,
                     Z_COLOR_RED);
}

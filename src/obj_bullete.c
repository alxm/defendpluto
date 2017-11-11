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
#include "util_collision.h"
#include "util_fix.h"
#include "util_fps.h"
#include "util_graphics.h"
#include "util_pool.h"
#include "util_screen.h"
#include "obj_bullete.h"
#include "obj_player.h"

void z_bullete_init(ZBulletE* Bullet, ZFix X, ZFix Y)
{
    Bullet->x = X;
    Bullet->y = Y;
}

bool z_bullete_tick(ZPoolObject* Bullet)
{
    ZBulletE* bullet = (ZBulletE*)Bullet;

    bullet->y = zf(bullet->y + z_fix_itofix(2));

    if(z_fix_fixtoi(bullet->y) >= Z_HEIGHT) {
        return false;
    }

    bool hit = z_collision_boxAndBox(z_fix_fixtoi(bullet->x),
                                     z_fix_fixtoi(bullet->y),
                                     2,
                                     4,
                                     z_fix_fixtoi(z_player.x),
                                     z_fix_fixtoi(z_player.y),
                                     z_player.w,
                                     z_player.h);

    if(hit) {
        z_player_takeDamage(256);
        z_screen_shake(Z_FPS / 15);
    }

    return !hit;
}

void z_bullete_draw(ZPoolObject* Bullet)
{
    ZBulletE* bullet = (ZBulletE*)Bullet;

    int8_t x = z_fix_fixtoi(bullet->x);
    int8_t y = z_fix_fixtoi(bullet->y);

    z_draw_rectangle(i8(x - 1 + z_screen_xShake),
                     i8(y - 2 + z_screen_yShake),
                     2,
                     4,
                     Z_COLOR_RED);
}

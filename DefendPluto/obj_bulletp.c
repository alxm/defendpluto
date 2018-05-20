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
#include "obj_bulletp.h"

#include "obj_enemy.h"
#include "obj_player.h"
#include "util_fps.h"
#include "util_screen.h"

struct ZBulletP {
    ZPoolObjHeader poolObject;
    ZFix x, y;
    uint8_t damage : 3;
};

Z_POOL_DECLARE(ZBulletP, 4, g_pool);

void z_bulletp_setup(void)
{
    z_pool_register(Z_POOL_BULLETP, g_pool);
}

void z_bulletp_init(ZBulletP* Bullet, ZFix X, ZFix Y, uint8_t Damage)
{
    Bullet->x = X;
    Bullet->y = Y;
    Bullet->damage = u3(Damage);
}

bool z_bulletp_tick(ZPoolObjHeader* Bullet, void* Context)
{
    Z_UNUSED(Context);

    ZBulletP* bullet = (ZBulletP*)Bullet;

    bullet->y = zf(bullet->y - z_fix_fromInt(2));

    if(bullet->y < 0) {
        return false;
    }

    return !z_enemy_checkCollisions(bullet->x,
                                    bullet->y,
                                    2,
                                    4,
                                    bullet->damage);
}

void z_bulletp_draw(ZPoolObjHeader* Bullet)
{
    ZBulletP* bullet = (ZBulletP*)Bullet;

    int16_t x = z_fix_toInt(bullet->x);
    int16_t y = z_fix_toInt(bullet->y);

    z_draw_rectangle(i16(x - 1 + z_screen_getXShake()),
                     i16(y - 2 + z_screen_getYShake()),
                     2,
                     4,
                     z_fps_getCounter() & 4 ? Z_COLOR_RED : Z_COLOR_YELLOW);
}

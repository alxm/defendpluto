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
#include "obj_bullete.h"

#include "obj_player.h"
#include "util_collision.h"

struct ZBulletE {
    ZPoolObjHeader poolObject;
    ZFix x, y;
    uint8_t angle : 4;
    uint8_t speed : 1;
    uint8_t damage : 3;
};

Z_POOL_DECLARE(ZBulletE, 16, g_pool);

void z_bullete_setup(void)
{
    z_pool_register(Z_POOL_BULLETE, g_pool);
}

void z_bullete_init(ZBulletE* Bullet, ZFix X, ZFix Y, uint8_t Angle, bool ExtraSpeed, uint8_t Damage)
{
    Bullet->x = X;
    Bullet->y = Y;
    Bullet->angle = Z_ANGLE_TO_U4(Angle);
    Bullet->speed = ExtraSpeed;
    Bullet->damage = u3(Damage);
}

bool z_bullete_tick(ZPoolObjHeader* Bullet, void* Context)
{
    Z_UNUSED(Context);

    ZBulletE* bullet = (ZBulletE*)Bullet;

    uint8_t angle = Z_U4_TO_ANGLE(bullet->angle);
    ZFix cos = z_fix_cos(angle);
    ZFix sin = z_fix_sin(angle);

    bullet->x = zf(bullet->x + cos * (1 + bullet->speed));
    bullet->y = zf(bullet->y - sin * (1 + bullet->speed));

    if(bullet->x < 0 || bullet->x >= Z_SCREEN_W * Z_FIX_ONE
        || bullet->y < 0 || bullet->y >= Z_SCREEN_H * Z_FIX_ONE) {

        return false;
    }

    return !z_collision_checkPlayer(bullet->x, bullet->y, 2, 3, bullet->damage);
}

void z_bullete_draw(ZPoolObjHeader* Bullet)
{
    ZBulletE* bullet = (ZBulletE*)Bullet;
    uint8_t angle = Z_U4_TO_ANGLE(bullet->angle);
    uint8_t frame = u8(Z_ANGLE_WRAP(angle + Z_ANGLE_045 / 2) / Z_ANGLE_045);

    z_sprite_blitCentered(Z_SPRITE_BULLETS,
                          z_fix_toInt(bullet->x),
                          z_fix_toInt(bullet->y),
                          frame);
}

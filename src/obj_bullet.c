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

#include "shared.h"
#include "util_pool.h"
#include "obj_bullet.h"

void z_bullet_init(ZBullet* Bullet, int8_t X, int8_t Y, int8_t Dy)
{
    Bullet->x = X;
    Bullet->y = Y;
    Bullet->dy = Dy;
}

bool z_bullet_tick(ZPoolObject* Bullet)
{
    ZBullet* bullet = (ZBullet*)Bullet;

    bullet->y = (int8_t)(bullet->y + bullet->dy);

    return (bullet->dy < 0 && bullet->y < 0)
        || (bullet->dy > 0 && bullet->y >= S_HEIGHT);
}

void z_bullet_draw(ZPoolObject* Bullet)
{
    ZBullet* bullet = (ZBullet*)Bullet;

    s_draw_rectangle(bullet->x - 1, bullet->y - 2, 2, 4, true);
}

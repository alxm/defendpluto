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
#include "util_fix.h"
#include "util_pool.h"
#include "obj_bullet.h"

void z_bullet_init(ZBullet* Bullet, fix X, fix Y, int Dy)
{
    Bullet->x = X;
    Bullet->y = Y;
    Bullet->speed = 2 << FIX_PRECISION_BITS;
    Bullet->dy = Dy;
}

bool z_bullet_tick(ZBullet* Bullet)
{
    Bullet->y = (fix)(Bullet->y + Bullet->dy * Bullet->speed);

    return (Bullet->dy < 0 && Bullet->y < 0)
        || (Bullet->dy > 0 && Bullet->y >> FIX_PRECISION_BITS >= S_HEIGHT);
}

void z_bullet_draw(ZBullet* Bullet)
{
    s_draw_rectangle(Bullet->x - 1,
                     (Bullet->y >> FIX_PRECISION_BITS) - 2,
                     2,
                     4,
                     true);
}

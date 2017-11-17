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
#include "util_enemy.h"
#include "util_fix.h"
#include "util_fps.h"
#include "util_pool.h"
#include "util_random.h"
#include "util_screen.h"
#include "obj_bullete.h"
#include "obj_enemy.h"

static void attack_none(ZEnemy* Enemy)
{
    Z_UNUSED(Enemy);
}

static void attack_random(ZEnemy* Enemy)
{
    if(Enemy->attack.counter-- > 0) {
        return;
    }

    ZBulletE* b = z_pool_alloc(Z_POOL_BULLETE);

    if(b) {
        z_bullete_init(b,
                       zf(Enemy->x + z_fix_itofix(z_screen_getXShake())),
                       Enemy->y);

        Enemy->attack.counter = u8(Z_FPS + z_random_uint8(4 * Z_FPS));
    } else {
        Enemy->attack.counter = 0;
    }
}

ZEnemyCallback z_enemy__attack[Z_ATTACK_NUM] = {
    attack_none,
    attack_random,
};

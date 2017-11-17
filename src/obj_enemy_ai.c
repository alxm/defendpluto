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
#include "util_pool.h"
#include "util_screen.h"
#include "obj_enemy.h"

static void ai_asteroid(ZEnemy* Enemy)
{
    Z_UNUSED(Enemy);
}

static void ai_ship0(ZEnemy* Enemy)
{
    if(z_fix_fixtoi(Enemy->y) > Z_HEIGHT / 4) {
        Enemy->fly.id = Z_FLY_ZIGZAG;
        Enemy->attack.id = Z_ATTACK_RANDOM;
    }
}

static void ai_ship1(ZEnemy* Enemy)
{
    Enemy->fly.id = Z_FLY_CURVE;
    Enemy->attack.id = Z_ATTACK_RANDOM;
}

static void ai_ship2(ZEnemy* Enemy)
{
    Enemy->fly.id = Z_FLY_LINE;
    Enemy->attack.id = Z_ATTACK_RANDOM;

    if(Enemy->angle == Z_ANGLE_270) {
        if(z_fix_fixtoi(Enemy->x) < Z_WIDTH / 2) {
            Enemy->angle = Z_ANGLE_315;
        } else {
            Enemy->angle = Z_ANGLE_225;
        }
    }
}

ZEnemyCallback z_enemy__ai[Z_ENEMY_NUM] = {
    ai_asteroid,
    ai_ship0,
    ai_ship1,
    ai_ship2,
};

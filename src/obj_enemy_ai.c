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
#include "util_enemy.h"
#include "util_fix.h"
#include "util_fps.h"
#include "util_pool.h"
#include "util_screen.h"
#include "obj_enemy.h"

#define DONE_STATE 0xf

static bool ai_asteroid(ZEnemy* Enemy)
{
    Z_UNUSED(Enemy);

    return true;
}

static bool ai_ship0(ZEnemy* Enemy)
{
    switch(Enemy->ai.state) {
        case 0: {
            if(Enemy->ai.flags & 1) {
                z_enemy_setAttack(Enemy, Z_ATTACK_STRAIGHT);
            }

            Enemy->ai.state = DONE_STATE;
        } break;

        case 1: {
            if(z_fix_fixtoi(Enemy->y) > 6) {
                Enemy->angle = Z_ANGLE_225;
                Enemy->ai.state = DONE_STATE;
            }
        } break;

        case 2: {
            if(z_fix_fixtoi(Enemy->y) > 6) {
                Enemy->angle = Z_ANGLE_315;
                Enemy->ai.state = DONE_STATE;
            }
        } break;

        case 3: {
            if(z_fix_fixtoi(Enemy->y) > 16) {
                Enemy->angle = Z_ANGLE_225;
                Enemy->ai.state = 0;
            }
        } break;

        case 4: {
            if(z_fix_fixtoi(Enemy->y) > 16) {
                Enemy->angle = Z_ANGLE_315;
                Enemy->ai.state = 0;
            }
        } break;
    }

    return true;
}

static bool ai_ship1(ZEnemy* Enemy)
{
    switch(Enemy->ai.state) {
        case 0: {
            Enemy->angle = Z_ANGLE_327;
        } break;

        case 1: {
            Enemy->angle = Z_ANGLE_202;
        } break;
    }

    return true;
}

static bool ai_ship2(ZEnemy* Enemy)
{
    if(!Z_EVERY_N_DS(20)) {
        return true;
    }

    switch(Enemy->ai.state) {
        case 0: {
            z_enemy_setFly(Enemy, Z_FLY_ZIGZAG);
            z_enemy_setAttack(Enemy, Z_ATTACK_TARGET);
            Enemy->ai.state = 1;
        } break;

        case 1: {
            z_enemy_setFly(Enemy, Z_FLY_LINE);
            z_enemy_setAttack(Enemy, Z_ATTACK_STRAIGHT);
            Enemy->ai.state = 0;
        } break;
    }

    return true;
}

ZEnemyCallback z_enemy_aiTable[Z_ENEMY_NUM] = {
    ai_asteroid,
    ai_ship0,
    ai_ship1,
    ai_ship2,
};

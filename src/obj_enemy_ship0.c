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
#include "util_fix.h"
#include "util_pool.h"
#include "obj_enemy.h"

bool z_enemy_ai_ship0(ZEnemy* Enemy)
{
    switch(Enemy->ai.state) {
        case 0: {
            if(Enemy->ai.flags & 1) {
                z_enemy_setAttack(Enemy, Z_ATTACK_STRAIGHT);
            }

            Enemy->ai.state = Z_DONE_STATE;
        } break;

        case 1: {
            if(z_fix_fixtoi(Enemy->y) > 6) {
                Enemy->angle = Z_ANGLE_225;
                Enemy->ai.state = Z_DONE_STATE;
            }
        } break;

        case 2: {
            if(z_fix_fixtoi(Enemy->y) > 6) {
                Enemy->angle = Z_ANGLE_315;
                Enemy->ai.state = Z_DONE_STATE;
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

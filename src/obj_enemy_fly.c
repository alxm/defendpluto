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

static void fly_line(ZEnemy* Enemy)
{
    Z_UNUSED(Enemy);
}

static void fly_zigzag(ZEnemy* Enemy)
{
    switch(Enemy->fly.state) {
        case 0: {
            if(z_fix_fixtoi(Enemy->y) > Z_HEIGHT / 8) {
                if(Enemy->fly.flipX) {
                    Enemy->angle = Z_ANGLE_225;
                } else {
                    Enemy->angle = Z_ANGLE_315;
                }

                Enemy->fly.state = 1;
            }
        } break;

        case 1: {
            if(Enemy->angle == Z_ANGLE_225) {
                Enemy->angle = Z_ANGLE_315;
            } else {
                Enemy->angle = Z_ANGLE_225;
            }
        } break;
    }
}

static void fly_curve(ZEnemy* Enemy)
{
    int8_t angleInc = 0;

    switch(Enemy->fly.state) {
        case 0: {
            if(Enemy->fly.flipX) {
                angleInc = -1;
                Enemy->fly.state = 1;
            } else {
                angleInc = 1;
                Enemy->fly.state = 2;
            }
        } break;

        case 1: {
            if(Enemy->angle <= Z_ANGLE_225) {
                Enemy->fly.state = 2;
            } else {
                angleInc = -1;
            }
        } break;

        case 2: {
            if(Enemy->angle >= Z_ANGLE_315) {
                Enemy->fly.state = 1;
            } else {
                angleInc = 1;
            }
        } break;
    }

    Enemy->angle = Z_ANGLE_WRAP(Enemy->angle + angleInc);
}

ZEnemyFlyPattern z_enemy_flyTable[Z_FLY_NUM] = {
    {fly_line, Z_DS_TO_FRAMES(0)},
    {fly_zigzag, Z_DS_TO_FRAMES(10)},
    {fly_curve, Z_DS_TO_FRAMES(1)},
};

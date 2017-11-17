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
#include "util_screen.h"
#include "obj_enemy.h"

static void fly_down(ZEnemy* Enemy)
{
    Z_UNUSED(Enemy);
}

static void fly_zigzag(ZEnemy* Enemy)
{
    bool expired = false;

    if(Enemy->fly.counter-- == 0) {
        Enemy->fly.counter = u8(Z_FPS + Enemy->fly.mod.delay * Z_FPS / 10);
        expired = true;
    }

    switch(Enemy->fly.state) {
        case 0: {
            if(z_fix_fixtoi(Enemy->y) > Z_HEIGHT / 8) {
                if(Enemy->fly.mod.flipX) {
                    Enemy->angle = Z_ANGLE_225;
                } else {
                    Enemy->angle = Z_ANGLE_315;
                }

                Enemy->fly.state = 1;
            }
        } break;

        case 1: {
            if(expired) {
                if(Enemy->angle == Z_ANGLE_225) {
                    Enemy->angle = Z_ANGLE_315;
                } else {
                    Enemy->angle = Z_ANGLE_225;
                }
            }
        } break;
    }
}

static void fly_curve(ZEnemy* Enemy)
{
    bool expired = false;
    int8_t angleInc = 0;

    if(Enemy->fly.counter-- == 0) {
        Enemy->fly.counter = u8(4 * Enemy->fly.mod.delay);
        expired = true;
    }

    switch(Enemy->fly.state) {
        case 0: {
            if(Enemy->fly.mod.flipX) {
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
            } else if(expired) {
                angleInc = -1;
            }
        } break;

        case 2: {
            if(Enemy->angle >= Z_ANGLE_315) {
                Enemy->fly.state = 1;
            } else if(expired) {
                angleInc = 1;
            }
        } break;
    }

    Enemy->angle = Z_ANGLE_WRAP(Enemy->angle + angleInc);
}

ZEnemyCallback z_enemy__fly[Z_FLY_NUM] = {
    fly_down,
    fly_zigzag,
    fly_curve,
};

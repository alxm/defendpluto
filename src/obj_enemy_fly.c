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
    Enemy->angle = Z_ANGLE_270;
}

static void fly_zigzag(ZEnemy* Enemy)
{
    bool expired = Enemy->fly.counter == 0;

    if(expired) {
        Enemy->fly.counter = Z_FPS;
    } else {
        Enemy->fly.counter--;
    }

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
    bool expired = Enemy->fly.counter == 0;

    if(expired) {
        Enemy->fly.counter = 1;
    } else {
        Enemy->fly.counter--;
    }

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
    fly_line,
    fly_zigzag,
    fly_curve,
};

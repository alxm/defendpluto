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
#include "util_fps.h"
#include "util_pool.h"
#include "util_screen.h"
#include "obj_enemy.h"

void z_enemy_ai_ship2(ZEnemy* Enemy)
{
    Z_AI {
        Z_AI_STATE(0) {
            z_enemy_attack(Enemy, Z_ATTACK_TARGET);

            switch(Enemy->fly.state) {
                case 0: {
                    if(z_fix_fixtoi(Enemy->y) > Z_HEIGHT / 8) {
                        Z_AI_FLAG(3) {
                            Enemy->angle = Z_ANGLE_225;
                        } else {
                            Enemy->angle = Z_ANGLE_315;
                        }

                        Enemy->fly.state = 1;
                    } else {
                        Enemy->angle = Z_ANGLE_270;
                    }
                } break;

                case 1: {
                    Z_AI_FLY_COUNTER_BLOCK(Enemy, 10);

                    if(Enemy->angle == Z_ANGLE_225) {
                        Enemy->angle = Z_ANGLE_315;
                    } else {
                        Enemy->angle = Z_ANGLE_225;
                    }
                } break;
            }
        }

        Z_AI_STATE(1) {
            z_enemy_attack(Enemy, Z_ATTACK_STRAIGHT);

            int8_t angleInc = 0;

            switch(Enemy->fly.state) {
                case 0: {
                    Z_AI_FLAG(3) {
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

            Z_AI_FLY_COUNTER_BLOCK(Enemy, 1);

            Enemy->angle = Z_ANGLE_WRAP(Enemy->angle + angleInc);
        }

        Z_AI_STATE(2) {
            z_enemy_attack(Enemy, Z_ATTACK_TARGET);

            switch(Enemy->fly.state) {
                case 0: {
                    if(z_fix_fixtoi(Enemy->y) >= Z_HEIGHT / 2) {
                        Enemy->angle = Z_ANGLE_000;
                        Enemy->fly.state = 1;
                    }
                } break;

                case 1: {
                    Z_AI_FLY_COUNTER_BLOCK(Enemy, 10);

                    Enemy->angle = Z_ANGLE_090;
                    Enemy->fly.state = 2;
                } break;

                case 2: {
                    Z_AI_FLY_COUNTER_BLOCK(Enemy, 10);

                    Enemy->angle = Z_ANGLE_180;
                    Enemy->fly.state = 3;
                } break;

                case 3: {
                    Z_AI_FLY_COUNTER_BLOCK(Enemy, 10);

                    Enemy->angle = Z_ANGLE_270;
                    Enemy->fly.state = 4;
                } break;

                case 4: {
                    Z_AI_FLY_COUNTER_BLOCK(Enemy, 10);

                    Enemy->angle = Z_ANGLE_000;
                    Enemy->fly.state = 1;
                } break;
            }
        }
    }
}

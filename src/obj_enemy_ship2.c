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

            Z_FLY {
                Z_FLY_STATE(0) {
                    if(z_fix_fixtoi(Enemy->y) > Z_HEIGHT / 8) {
                        Z_AI_FLAG(3) {
                            Enemy->angle = Z_ANGLE_225;
                        } else {
                            Enemy->angle = Z_ANGLE_315;
                        }

                        Z_FLY_GO(1);
                    } else {
                        Enemy->angle = Z_ANGLE_270;
                    }
                }

                Z_FLY_STATE(1) {
                    Z_FLY_EVERY_DS(10) {
                        if(Enemy->angle == Z_ANGLE_225) {
                            Enemy->angle = Z_ANGLE_315;
                        } else {
                            Enemy->angle = Z_ANGLE_225;
                        }
                    }
                }
            }
        }

        Z_AI_STATE(1) {
            z_enemy_attack(Enemy, Z_ATTACK_STRAIGHT);

            int8_t angleInc = 0;

            Z_FLY {
                Z_FLY_STATE(0) {
                    Z_AI_FLAG(3) {
                        angleInc = -1;
                        Z_FLY_GO(1);
                    } else {
                        angleInc = 1;
                        Z_FLY_GO(2);
                    }
                }

                Z_FLY_STATE(1) {
                    if(Enemy->angle <= Z_ANGLE_225) {
                        Z_FLY_GO(2);
                    } else {
                        angleInc = -1;
                    }
                }

                Z_FLY_STATE(2) {
                    if(Enemy->angle >= Z_ANGLE_315) {
                        Z_FLY_GO(1);
                    } else {
                        angleInc = 1;
                    }
                }
            }

            Z_FLY_EVERY_DS(1) {
                Enemy->angle = Z_ANGLE_WRAP(Enemy->angle + angleInc);
            }
        }

        Z_AI_STATE(2) {
            z_enemy_attack(Enemy, Z_ATTACK_TARGET);

            Z_FLY {
                Z_FLY_STATE(0) {
                    if(z_fix_fixtoi(Enemy->y) >= Z_HEIGHT / 2) {
                        Enemy->angle = Z_ANGLE_000;
                        Z_FLY_GO(1);

                        Z_FLY_COUNTER_SET(20);
                    }
                }

                Z_FLY_STATE(1) {
                    Z_FLY_EVERY_DS(10) {
                        Enemy->angle = Z_ANGLE_090;
                        Z_FLY_GO(2);
                    }
                }

                Z_FLY_STATE(2) {
                    Z_FLY_EVERY_DS(20) {
                        Enemy->angle = Z_ANGLE_180;
                        Z_FLY_GO(3);
                    }
                }

                Z_FLY_STATE(3) {
                    Z_FLY_EVERY_DS(10) {
                        Enemy->angle = Z_ANGLE_270;
                        Z_FLY_GO(4);
                    }
                }

                Z_FLY_STATE(4) {
                    Z_FLY_EVERY_DS(20) {
                        Enemy->angle = Z_ANGLE_000;
                        Z_FLY_GO(1);
                    }
                }
            }
        }
    }
}

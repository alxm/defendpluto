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
#include "util_collision.h"
#include "util_enemy.h"
#include "util_fix.h"
#include "util_fps.h"
#include "util_graphics.h"
#include "util_pool.h"
#include "util_random.h"
#include "util_screen.h"
#include "obj_bullete.h"
#include "obj_circle.h"
#include "obj_enemy.h"
#include "obj_particle.h"

static struct {
    bool hit;
    bool allowMultiple;
    int8_t x, y, w, h;
} g_coll;

static void fly_down(ZEnemy* Enemy)
{
    Z_UNUSED(Enemy);
}

static void fly_zigzag(ZEnemy* Enemy)
{
    bool expired = false;

    if(Enemy->counters[0]-- == 0) {
        Enemy->counters[0] = u8(32 + 4 * Enemy->flyArgs.delay);
        expired = true;
    }

    switch(Enemy->flyState) {
        case 0: {
            if(z_fix_fixtoi(Enemy->y) > Z_HEIGHT / 8) {
                if(Enemy->flyArgs.flipX) {
                    Enemy->angle = Z_ANGLE_225;
                } else {
                    Enemy->angle = Z_ANGLE_315;
                }

                Enemy->flyState = 1;
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

    if(Enemy->counters[0]-- == 0) {
        Enemy->counters[0] = u8(4 * Enemy->flyArgs.delay);
        expired = true;
    }

    switch(Enemy->flyState) {
        case 0: {
            if(Enemy->flyArgs.flipX) {
                angleInc = -1;
                Enemy->flyState = 1;
            } else {
                angleInc = 1;
                Enemy->flyState = 2;
            }
        } break;

        case 1: {
            if(Enemy->angle <= Z_ANGLE_225) {
                Enemy->flyState = 2;
            } else if(expired) {
                angleInc = -1;
            }
        } break;

        case 2: {
            if(Enemy->angle >= Z_ANGLE_315) {
                Enemy->flyState = 1;
            } else if(expired) {
                angleInc = 1;
            }
        } break;
    }

    Enemy->angle = Z_ANGLE_WRAP(Enemy->angle + angleInc);
}

static void (*g_fly[])(ZEnemy*) = {
    fly_down,
    fly_zigzag,
    fly_curve,
};

void z_enemy_init(ZEnemy* Enemy, int8_t X, int8_t Y, uint8_t TypeId, uint8_t FlyId, uint8_t Delay, uint8_t FlipX)
{
    Enemy->x = z_fix_itofix(X);
    Enemy->y = z_fix_itofix(Y);
    Enemy->angle = Z_ANGLE_270;
    Enemy->jetFlicker = false;
    Enemy->typeId = u4(TypeId);
    Enemy->frame = 0;
    Enemy->flyId = u4(FlyId);
    Enemy->flyState = 0;
    Enemy->flyArgs.delay = u4(Delay);
    Enemy->flyArgs.flipX = u4(FlipX);

    for(uint8_t i = 0; i < Z_ARRAY_LEN(Enemy->counters); i++) {
        Enemy->counters[i] = 0;
    }
}

bool z_enemy_tick(ZPoolObject* Enemy)
{
    ZEnemy* enemy = (ZEnemy*)Enemy;

    ZFix cos = z_fix_cos(enemy->angle);
    ZFix sin = z_fix_sin(enemy->angle);

    enemy->x = zf(enemy->x + z_fix_mul(cos, z_enemyData[enemy->typeId].speed));
    enemy->y = zf(enemy->y - z_fix_mul(sin, z_enemyData[enemy->typeId].speed));

    if(z_fps_isNthFrame(Z_FPS / 5)) {
        ZSprite* sprite = &z_enemyData[enemy->typeId].sprite;
        enemy->frame = u4((enemy->frame + 1) % sprite->numFrames);
    }

    enemy->jetFlicker = !enemy->jetFlicker;

    g_fly[enemy->flyId](enemy);

    if(z_enemyData[enemy->typeId].damage > 0 && enemy->counters[1]-- == 0) {
        ZBulletE* b = z_pool_alloc(Z_POOL_BULLETE);

        if(b) {
            z_bullete_init(b,
                           zf(enemy->x + z_fix_itofix(z_screen_getXShake())),
                           enemy->y);

            enemy->counters[1] = u8(Z_FPS + z_random_uint8(4 * Z_FPS));
        } else {
            enemy->counters[1] = 0;
        }
    }

    return z_fix_fixtoi(enemy->y) - z_enemyData[enemy->typeId].h / 2 < Z_HEIGHT;
}

void z_enemy_draw(ZPoolObject* Enemy)
{
    ZEnemy* enemy = (ZEnemy*)Enemy;
    int8_t x = z_fix_fixtoi(enemy->x);
    int8_t y = z_fix_fixtoi(enemy->y);
    ZSprite* sprite = &z_enemyData[enemy->typeId].sprite;

    if(enemy->jetFlicker) {
        z_enemy_drawJets(enemy->typeId, x, y);
    }

    z_sprite_blitCentered(sprite,
                          i8(x + z_screen_getXShake()),
                          i8(y + z_screen_getYShake()),
                          enemy->frame);
}

static bool checkCollision(ZPoolObject* Enemy)
{
    if(!g_coll.allowMultiple && g_coll.hit) {
        return true;
    }

    ZEnemy* enemy = (ZEnemy*)Enemy;

    bool hit = z_collision_boxAndBox(g_coll.x,
                                     g_coll.y,
                                     g_coll.w,
                                     g_coll.h,
                                     z_fix_fixtoi(enemy->x),
                                     z_fix_fixtoi(enemy->y),
                                     z_enemyData[enemy->typeId].w,
                                     z_enemyData[enemy->typeId].h);

    if(hit) {
        for(int8_t i = 4; i--; ) {
            ZParticle* p = z_pool_alloc(Z_POOL_PARTICLE);

            if(p == NULL) {
                break;
            }

            z_particle_init(p, enemy->x, enemy->y);
        }

        ZCircle* c = z_pool_alloc(Z_POOL_CIRCLE);

        if(c) {
            z_circle_init(c, z_fix_fixtoi(enemy->x), z_fix_fixtoi(enemy->y));
        }

        z_screen_shake(Z_FPS / 3);
    }

    g_coll.hit |= hit;

    return !hit;
}

bool z_enemy_checkCollisions(int8_t X, int8_t Y, int8_t W, int8_t H, bool AllowMultipleCollisions)
{
    g_coll.x = X;
    g_coll.y = Y;
    g_coll.w = W;
    g_coll.h = H;
    g_coll.hit = false;
    g_coll.allowMultiple = AllowMultipleCollisions;

    z_pool_tick(Z_POOL_ENEMY, checkCollision);

    return g_coll.hit;
}

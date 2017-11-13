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

static ZFix ai_down(ZEnemy* Enemy)
{
    Z_UNUSED(Enemy);

    return 0;
}

static ZFix ai_zigzag(ZEnemy* Enemy)
{
    switch(Enemy->aiState) {
        case 0: {
            if(z_fix_fixtoi(Enemy->y) > Z_HEIGHT / 8) {
                if(Enemy->aiArgs & 1) {
                    Enemy->angle = Z_FIX_ANGLE_225;
                } else {
                    Enemy->angle = Z_FIX_ANGLE_315;
                }

                Enemy->aiState = 1;
                Enemy->aiCounter = Z_FPS / 2;
            }
        } break;

        case 1: {
            if(Enemy->aiCounter-- == 0) {
                if(Enemy->angle == Z_FIX_ANGLE_225) {
                    Enemy->angle = Z_FIX_ANGLE_315;
                } else {
                    Enemy->angle = Z_FIX_ANGLE_225;
                }

                Enemy->aiCounter = Z_FPS;
            }
        } break;
    }

    return 0;
}

static ZFix ai_curve(ZEnemy* Enemy)
{
    ZFix angleInc = 0;

    switch(Enemy->aiState) {
        case 0: {
            if(Enemy->aiArgs & 1) {
                angleInc = -Z_FIX_ONE;
                Enemy->aiState = 1;
            } else {
                angleInc = Z_FIX_ONE;
                Enemy->aiState = 2;
            }
        } break;

        case 1: {
            if(Enemy->angle <= Z_FIX_ANGLE_225) {
                Enemy->aiState = 2;
            } else {
                angleInc = -Z_FIX_ONE;
            }
        } break;

        case 2: {
            if(Enemy->angle >= Z_FIX_ANGLE_315) {
                Enemy->aiState = 1;
            } else {
                angleInc = Z_FIX_ONE;
            }
        } break;
    }

    return angleInc;
}

static ZFix (*g_ai[])(ZEnemy*) = {
    ai_down,
    ai_zigzag,
    ai_curve,
};

void z_enemy_init(ZEnemy* Enemy, int8_t X, int8_t Y, uint8_t TypeId, uint8_t AiId, uint8_t AiArgs)
{
    Enemy->x = z_fix_itofix(X);
    Enemy->y = z_fix_itofix(Y);
    Enemy->angle = Z_FIX_ANGLE_270;
    Enemy->typeId = TypeId;
    Enemy->frame = 0;
    Enemy->aiId = AiId;
    Enemy->aiArgs = AiArgs;
    Enemy->aiState = 0;
    Enemy->aiCounter = 0;
    Enemy->jetFlicker = false;
}

bool z_enemy_tick(ZPoolObject* Enemy)
{
    ZEnemy* enemy = (ZEnemy*)Enemy;
    ZFix angleInc = g_ai[enemy->aiId](enemy);

    ZFix cos = z_fix_cos(u8(z_fix_fixtoi(enemy->angle)));
    ZFix sin = z_fix_sin(u8(z_fix_fixtoi(enemy->angle)));

    ZFix dx = z_fix_mul(cos, z_enemyData[enemy->typeId].speed);
    ZFix dy = z_fix_mul(sin, z_enemyData[enemy->typeId].speed);

    enemy->x = zf(enemy->x + dx);
    enemy->y = zf(enemy->y - dy);
    enemy->angle = zf((enemy->angle + angleInc) & (Z_FIX_NUM_ANGLESF - 1));

    if(z_fps_isNthFrame(6)) {
        ZSprite* sprite = &z_enemyData[enemy->typeId].sprite;
        enemy->frame = u8((enemy->frame + 1) % sprite->numFrames);
    }

    enemy->jetFlicker = !enemy->jetFlicker;

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
                          i8(x + z_screen_xShake),
                          i8(y + z_screen_yShake),
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
        for(int8_t i = Z_PARTICLE_POOL_NUM; i--; ) {
            ZParticle* p = z_pool_alloc(Z_POOL_PARTICLE);

            if(p == NULL) {
                break;
            }

            z_particle_init(p,
                            enemy->x,
                            enemy->y,
                            u8(Z_FPS / 8 + z_random_uint8(Z_FPS / 4)));
        }

        ZCircle* c = z_pool_alloc(Z_POOL_CIRCLE);

        if(c) {
            z_circle_init(c,
                          z_fix_fixtoi(enemy->x),
                          z_fix_fixtoi(enemy->y),
                          5,
                          Z_FIX_ONE);
        }

        c = z_pool_alloc(Z_POOL_CIRCLE);

        if(c) {
            z_circle_init(c,
                          z_fix_fixtoi(enemy->x),
                          z_fix_fixtoi(enemy->y),
                          2,
                          Z_FIX_ONE / 2);
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

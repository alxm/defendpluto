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
#include "util_collision.h"
#include "util_enemy.h"
#include "util_fix.h"
#include "util_fps.h"
#include "util_graphics.h"
#include "util_pool.h"
#include "util_screen.h"
#include "obj_circle.h"
#include "obj_enemy.h"
#include "obj_particle.h"

static struct {
    bool hit;
    bool allowMultiple;
    int16_t x, y;
    int8_t w, h;
} g_coll;

void z_enemy_init(ZEnemy* Enemy, int16_t X, int16_t Y, uint8_t TypeId, uint8_t AiState, uint8_t AiFlags)
{
    Enemy->x = z_fix_itofix(X);
    Enemy->y = z_fix_itofix(Y);
    Enemy->angle = Z_ANGLE_270;
    Enemy->jetFlicker = false;
    Enemy->typeId = u4(TypeId);
    Enemy->frame = 0;
    Enemy->ai.state = u4(AiState);
    Enemy->ai.flags = u4(AiFlags);

    z_enemy_setFly(Enemy, Z_FLY_LINE);
    z_enemy_setAttack(Enemy, Z_ATTACK_NONE);
}

bool z_enemy_tick(ZPoolObject* Enemy)
{
    ZEnemy* enemy = (ZEnemy*)Enemy;

    ZFix cos = z_fix_cos(enemy->angle);
    ZFix sin = z_fix_sin(enemy->angle);
    ZFix speed = z_enemyData[enemy->typeId].speedShift;

    enemy->x = zf(enemy->x + (cos >> speed));
    enemy->y = zf(enemy->y - (sin >> speed));

    if(Z_EVERY_N_DS(2)) {
        ZSprite* sprite = &z_enemyData[enemy->typeId].sprite;
        enemy->frame = u4((enemy->frame + 1) % sprite->numFrames);
    }

    enemy->jetFlicker = !enemy->jetFlicker;

    bool alive = z_enemy_aiTable[enemy->typeId](enemy);

    if(alive) {
        if(enemy->fly.counter-- == 0) {
            enemy->fly.counter = z_enemy_flyTable[enemy->fly.id].framesPeriod;
            alive = z_enemy_flyTable[enemy->fly.id].callback(enemy);
        }

        if(enemy->attack.counter == 0) {
            z_enemy_attackTable[enemy->attack.id](enemy);
        } else if(Z_EVERY_N_DS(2)) {
            enemy->attack.counter--;
        }
    }

    return alive;
}

void z_enemy_draw(ZPoolObject* Enemy)
{
    ZEnemy* enemy = (ZEnemy*)Enemy;
    int16_t x = z_fix_fixtoi(enemy->x);
    int16_t y = z_fix_fixtoi(enemy->y);
    ZSprite* sprite = &z_enemyData[enemy->typeId].sprite;

    if(enemy->jetFlicker) {
        z_enemy_drawJets(enemy->typeId, x, y);
    }

    z_sprite_blitCentered(sprite,
                          i16(x + z_screen_getXShake()),
                          i16(y + z_screen_getYShake()),
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

        z_screen_shake(Z_DS_TO_FRAMES(3));
    }

    g_coll.hit |= hit;

    return !hit;
}

bool z_enemy_checkCollisions(int16_t X, int16_t Y, int8_t W, int8_t H, bool AllowMultipleCollisions)
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

void z_enemy_setFly(ZEnemy* Enemy, uint8_t FlyId)
{
    Enemy->fly.id = u4(FlyId);
    Enemy->fly.state = 0;
    Enemy->fly.flipX = 0;
    Enemy->fly.flipY = 0;
    Enemy->fly.counter = 0;
}

void z_enemy_setAttack(ZEnemy* Enemy, uint8_t AttackId)
{
    Enemy->attack.id = u4(AttackId);
    Enemy->attack.counter = 0;
}

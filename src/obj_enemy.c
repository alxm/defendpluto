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

static void nextFrame(ZEnemy* Enemy)
{
    if(z_fps_isNthFrame(6)) {
        ZSprite* sprite = &z_graphics.enemy[Enemy->sprite];

        Enemy->frame = (uint8_t)((Enemy->frame + 1) % sprite->numFrames);
    }
}

static void glideDown(ZEnemy* Enemy)
{
    Enemy->y = (ZFix)(Enemy->y + Z_FIX_ONE / 4);
}

static bool onScreen(ZEnemy* Enemy)
{
    ZSprite* sprite = &z_graphics.enemy[Enemy->sprite];

    return z_fix_fixtoi(Enemy->y) - z_sprite_getHeight(sprite) / 2 < Z_HEIGHT;
}

static bool ai_nobrain(ZEnemy* Enemy)
{
    nextFrame(Enemy);
    glideDown(Enemy);

    return onScreen(Enemy);
}

static bool ai_shoot(ZEnemy* Enemy)
{
    nextFrame(Enemy);
    glideDown(Enemy);

    if(z_fps_isNthFrame(Z_FPS)) {
        ZBulletE* b = z_pool_alloc(Z_POOL_BULLETE);

        if(b) {
            z_bullete_init(b,
                           (ZFix)(Enemy->x + z_fix_itofix(z_screen_xShake)),
                           Enemy->y);
        }
    }

    return onScreen(Enemy);
}

static bool (*g_ai[Z_AI_ID_NUM])(ZEnemy*) = {
    ai_nobrain,
    ai_shoot,
};

void z_enemy_init(ZEnemy* Enemy, int8_t X, int8_t Y, uint8_t Sprite, uint8_t AiId, uint8_t AiArgs)
{
    Enemy->x = z_fix_itofix(X);
    Enemy->y = z_fix_itofix(Y);
    Enemy->sprite = Sprite;
    Enemy->frame = 0;
    Enemy->aiId = AiId;
    Enemy->aiArgs = AiArgs;
    Enemy->jetFlicker = false;
}

bool z_enemy_tick(ZPoolObject* Enemy)
{
    ZEnemy* enemy = (ZEnemy*)Enemy;

    enemy->jetFlicker = !enemy->jetFlicker;

    return g_ai[enemy->aiId](enemy);
}

void z_enemy_draw(ZPoolObject* Enemy)
{
    ZEnemy* enemy = (ZEnemy*)Enemy;
    int8_t x = z_fix_fixtoi(enemy->x);
    int8_t y = z_fix_fixtoi(enemy->y);
    ZSprite* sprite = &z_graphics.enemy[enemy->sprite];

    if(enemy->jetFlicker) {
        z_graphics_drawJets(enemy->sprite, x, y);
    }

    z_sprite_blitCentered(sprite,
                          (int8_t)(x + z_screen_xShake),
                          (int8_t)(y + z_screen_yShake),
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
                                     6,
                                     6);

    if(hit) {
        for(int8_t i = Z_PARTICLE_POOL_NUM; i--; ) {
            ZParticle* p = z_pool_alloc(Z_POOL_PARTICLE);

            if(p == NULL) {
                break;
            }

            z_particle_init(p,
                            enemy->x,
                            enemy->y,
                            (uint8_t)(Z_FPS / 8
                                        + z_random_uint8(Z_FPS / 4)));
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

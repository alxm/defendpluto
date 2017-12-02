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
#include "util_fix.h"
#include "util_fps.h"
#include "util_graphics.h"
#include "util_pool.h"
#include "util_screen.h"
#include "obj_bullete.h"
#include "obj_circle.h"
#include "obj_enemy.h"
#include "obj_particle.h"
#include "obj_player.h"
#include "data_gfx_asteroid.h"
#include "data_gfx_enemy00.h"
#include "data_gfx_enemy01.h"
#include "data_gfx_enemy02.h"

static struct {
    bool hit;
    bool allowMultiple;
    int16_t x, y;
    int8_t w, h;
    uint8_t damage;
} g_coll;

ZEnemyData z_enemy_data[Z_ENEMY_NUM];

void z_enemy_setup(void)
{
    #define enemy(Index, Id, Ai, Width, Height, Health, Damage, Speed) \
        z_sprite_load(&z_enemy_data[Index].sprite, Id);                \
        z_enemy_data[Index].ai = Ai;                                   \
        z_enemy_data[Index].w = Width;                                 \
        z_enemy_data[Index].h = Height;                                \
        z_enemy_data[Index].health = Health;                           \
        z_enemy_data[Index].damage = Damage;                           \
        z_enemy_data[Index].speedShift = Speed;                        \

    enemy(Z_ENEMY_ASTEROID, asteroid, z_enemy_ai_asteroid, 8, 8, 3, 0, 2);
    enemy(Z_ENEMY_SHIP0,    enemy00,  z_enemy_ai_ship0,    7, 5, 1, 2, 1);
    enemy(Z_ENEMY_SHIP1,    enemy01,  z_enemy_ai_ship1,    7, 5, 1, 4, 1);
    enemy(Z_ENEMY_SHIP2,    enemy02,  z_enemy_ai_ship2,    7, 6, 2, 6, 2);
}

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
    Enemy->fly.state = 0;
    Enemy->fly.counter = 0;
    Enemy->attack.counter = 0;
    Enemy->health = z_enemy_data[TypeId].health;
}

bool z_enemy_tick(ZPoolObject* Enemy)
{
    ZEnemy* enemy = (ZEnemy*)Enemy;

    ZFix cos = z_fix_cos(enemy->angle);
    ZFix sin = z_fix_sin(enemy->angle);
    ZFix speed = z_enemy_data[enemy->typeId].speedShift;
    ZSprite* sprite = &z_enemy_data[enemy->typeId].sprite;

    enemy->x = zf(enemy->x + (cos >> speed));
    enemy->y = zf(enemy->y - (sin >> speed));

    Z_EVERY_DS(2) {
        enemy->frame = u4((enemy->frame + 1) % sprite->numFrames);
    }

    enemy->jetFlicker = !enemy->jetFlicker;

    z_enemy_data[enemy->typeId].ai(enemy);

    return z_fix_fixtoi(enemy->y) - z_sprite_getHeight(sprite) / 2 < Z_HEIGHT;
}

static void drawJets(uint8_t EnemyId, int16_t X, int16_t Y)
{
    int16_t x, y;
    int8_t w, h;

    switch(EnemyId) {
        case Z_ENEMY_SHIP0: {
            x = X;
            y = i16(Y - 2);
            z_draw_pixel(x, y, Z_COLOR_RED);
        } break;

        case Z_ENEMY_SHIP1: {
            x = i16(X - 3);
            y = i16(Y - 3);
            z_draw_pixel(x, y, Z_COLOR_RED);

            x = i16(X + 3);
            z_draw_pixel(x, y, Z_COLOR_RED);
        } break;

        case Z_ENEMY_SHIP2: {
            x = i16(X - 2);
            y = i16(Y - 4);
            w = 2;
            h = 1;
            z_draw_rectangle(x,
                             i16(y + z_screen_getYShake()),
                             w,
                             h,
                             Z_COLOR_RED);

            x = i16(X + 1);
            z_draw_rectangle(x,
                             i16(y + z_screen_getYShake()),
                             w,
                             h,
                             Z_COLOR_RED);
        } break;

        default: return;
    }
}

void z_enemy_draw(ZPoolObject* Enemy)
{
    ZEnemy* enemy = (ZEnemy*)Enemy;
    int16_t x = z_fix_fixtoi(enemy->x);
    int16_t y = z_fix_fixtoi(enemy->y);
    ZSprite* sprite = &z_enemy_data[enemy->typeId].sprite;

    if(enemy->jetFlicker) {
        drawJets(enemy->typeId, x, y);
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

    if(z_collision_boxAndBox(g_coll.x,
                             g_coll.y,
                             g_coll.w,
                             g_coll.h,
                             z_fix_fixtoi(enemy->x),
                             z_fix_fixtoi(enemy->y),
                             z_enemy_data[enemy->typeId].w,
                             z_enemy_data[enemy->typeId].h)) {

        g_coll.hit = true;

        if(enemy->health > g_coll.damage) {
            enemy->health = u8((enemy->health - g_coll.damage) & 3);
        } else {
            enemy->health = 0;

            ZCircle* c = z_pool_alloc(Z_POOL_CIRCLE);

            if(c) {
                z_circle_init(c, z_fix_fixtoi(enemy->x), z_fix_fixtoi(enemy->y));
            }

            z_screen_shake(Z_DS_TO_FRAMES(3));
        }

        for(int8_t i = 4; i--; ) {
            ZParticle* p = z_pool_alloc(Z_POOL_PARTICLE);

            if(p == NULL) {
                break;
            }

            z_particle_init(p, enemy->x, enemy->y);
        }
    }

    return enemy->health > 0;
}

bool z_enemy_checkCollisions(int16_t X, int16_t Y, int8_t W, int8_t H, uint8_t Damage, bool AllowMultipleCollisions)
{
    g_coll.x = X;
    g_coll.y = Y;
    g_coll.w = W;
    g_coll.h = H;
    g_coll.hit = false;
    g_coll.damage = Damage;
    g_coll.allowMultiple = AllowMultipleCollisions;

    z_pool_tick(Z_POOL_ENEMY, checkCollision);

    return g_coll.hit;
}

static void shoot(ZEnemy* Enemy, uint8_t Angle, bool ExtraSpeed)
{
    ZBulletE* b = z_pool_alloc(Z_POOL_BULLETE);

    if(b) {
        z_bullete_init(b,
                       zf(Enemy->x + z_fix_itofix(z_screen_getXShake())),
                       Enemy->y,
                       Angle,
                       ExtraSpeed,
                       z_enemy_data[Enemy->typeId].damage);
    }
}

void z_enemy_attack(ZEnemy* Enemy, uint8_t AttackId)
{
    if(Enemy->y < 0) {
        return;
    }

    if(Enemy->attack.counter-- == 0) {
        Enemy->attack.counter = Z_DS_TO_FRAMES(8);
    } else {
        return;
    }

    switch(AttackId) {
        case Z_ATTACK_STRAIGHT: {
            shoot(Enemy, Enemy->angle, false);
        } break;

        case Z_ATTACK_TARGET: {
            shoot(Enemy,
                  z_fix_atan(Enemy->x, Enemy->y, z_player.x, z_player.y),
                  false);
        } break;
    }
}

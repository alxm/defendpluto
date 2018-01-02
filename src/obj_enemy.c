/*
    Copyright 2017, 2018 Alex Margarit <alex@alxm.org>

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
#include "obj_player.h"

ZEnemyData z_enemy_data[Z_ENEMY_NUM];

void z_enemy_setup(void)
{
    #define enemy(Index, Sprite, Ai, Width, Height, Health, Damage, Speed) \
        z_enemy_data[Index].sprite = Sprite;                               \
        z_enemy_data[Index].ai = Ai;                                       \
        z_enemy_data[Index].w = Width;                                     \
        z_enemy_data[Index].h = Height;                                    \
        z_enemy_data[Index].health = Health;                               \
        z_enemy_data[Index].damage = Damage;                               \
        z_enemy_data[Index].speedShift = Speed;

    enemy(Z_ENEMY_ASTEROID, Z_SPRITE_ASTEROID, z_enemy_ai_asteroid, 8, 8, 3, 0, 2);
    enemy(Z_ENEMY_SHIP0,    Z_SPRITE_ENEMY00,  z_enemy_ai_ship0,    7, 5, 1, 2, 1);
    enemy(Z_ENEMY_SHIP1,    Z_SPRITE_ENEMY01,  z_enemy_ai_ship1,    7, 5, 1, 4, 1);
    enemy(Z_ENEMY_SHIP2,    Z_SPRITE_ENEMY02,  z_enemy_ai_ship2,    7, 6, 2, 6, 2);
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

bool z_enemy_tick(ZPoolObject* Enemy, void* Context)
{
    Z_UNUSED(Context);

    ZEnemy* enemy = (ZEnemy*)Enemy;

    uint8_t type = enemy->typeId;
    ZFix cos = z_fix_cos(enemy->angle);
    ZFix sin = z_fix_sin(enemy->angle);
    ZFix speed = z_enemy_data[type].speedShift;
    uint8_t sprite = z_enemy_data[type].sprite;

    enemy->x = zf(enemy->x + (cos >> speed));
    enemy->y = zf(enemy->y - (sin >> speed));

    Z_EVERY_DS(2) {
        enemy->frame = u4((enemy->frame + 1) % z_sprite_getNumFrames(sprite));
    }

    enemy->jetFlicker = !enemy->jetFlicker;

    z_enemy_data[type].ai(enemy);

    if(z_collision_checkPlayer(enemy->x,
                               enemy->y,
                               z_enemy_data[type].w,
                               z_enemy_data[type].h,
                               Z_SHIELD_MAX)) {

        z_enemy_takeDamage(enemy, UINT8_MAX);
    }

    return enemy->health > 0
        && z_fix_fixtoi(enemy->y) - z_sprite_getHeight(sprite) / 2 < Z_HEIGHT;
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

    if(enemy->jetFlicker) {
        drawJets(enemy->typeId, x, y);
    }

    z_sprite_blitCentered(z_enemy_data[enemy->typeId].sprite,
                          i16(x + z_screen_getXShake()),
                          i16(y + z_screen_getYShake()),
                          enemy->frame);
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

void z_enemy_takeDamage(ZEnemy* Enemy, uint8_t Damage)
{
    if(Enemy->health > Damage) {
        Enemy->health = u2(Enemy->health - Damage);
    } else {
        Enemy->health = 0;

        ZCircle* c = z_pool_alloc(Z_POOL_CIRCLE);

        if(c) {
            z_circle_init(c,
                          z_fix_fixtoi(Enemy->x),
                          z_fix_fixtoi(Enemy->y));
        }

        z_screen_shake(Z_DS_TO_FRAMES(3));
    }
}

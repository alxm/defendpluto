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
#include "util_fix.h"
#include "util_collision.h"
#include "util_effects.h"
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
    #define enemy(Index, Sprite, Width, Height, Health, Damage, Speed, AttackDs) \
        z_enemy_data[Index].w = Width;                                           \
        z_enemy_data[Index].h = Height;                                          \
        z_enemy_data[Index].health = Health;                                     \
        z_enemy_data[Index].damage = Damage;                                     \
        z_enemy_data[Index].speedShift = Speed;                                  \
        z_enemy_data[Index].attackPeriodDs = AttackDs;                           \
        z_enemy_data[Index].sprite = Sprite;

    enemy(Z_ENEMY_ASTEROID, Z_SPRITE_ASTEROID, 8, 8, 3, 0, 2, 10);
    enemy(Z_ENEMY_SHIP0,    Z_SPRITE_ENEMY00,  7, 5, 1, 2, 1, 30);
    enemy(Z_ENEMY_SHIP1,    Z_SPRITE_ENEMY01,  7, 5, 1, 4, 2, 30);
    enemy(Z_ENEMY_SHIP2,    Z_SPRITE_ENEMY02,  7, 6, 2, 6, 1, 30);
}

void z_enemy_init(ZEnemy* Enemy, int16_t X, int16_t Y, uint8_t TypeId, uint8_t FlyId, uint8_t AttackId)
{
    Enemy->x = z_fix_itofix(X);
    Enemy->y = z_fix_itofix(Y);
    Enemy->angle = Z_ANGLE_270;
    Enemy->jetFlicker = false;
    Enemy->frame = 0;
    Enemy->typeId = u4(TypeId);
    Enemy->flyId = u4(FlyId);
    Enemy->attackId = u4(AttackId);
    Enemy->flyCounter = 0;
    Enemy->attackCounter = z_enemy_data[TypeId].attackPeriodDs;
    Enemy->health = z_enemy_data[TypeId].health;
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

bool z_enemy_tick(ZPoolObject* Enemy, void* Context)
{
    Z_UNUSED(Context);

    ZEnemy* enemy = (ZEnemy*)Enemy;

    uint8_t type = enemy->typeId;
    ZFix cos = z_fix_cos(enemy->angle);
    ZFix sin = z_fix_sin(enemy->angle);
    ZFix speed = z_enemy_data[type].speedShift;
    uint8_t sprite = z_enemy_data[type].sprite;

    switch(enemy->flyId) {
        case Z_FLY_STILL: {
            // Do nothing
        } break;

        case Z_FLY_DOWN: {
            enemy->x = zf(enemy->x + (cos >> speed));
            enemy->y = zf(enemy->y - (sin >> speed));
        } break;
    }

    if(enemy->y >= 0 && enemy->attackCounter-- == 0) {
        enemy->attackCounter =
            Z_DS_TO_FRAMES(z_enemy_data[enemy->typeId].attackPeriodDs);

        switch(enemy->attackId) {
            case Z_ATTACK_FRONT: {
                shoot(enemy, enemy->angle, false);
            } break;

            case Z_ATTACK_TARGET: {
                shoot(enemy,
                      z_fix_atan(enemy->x, enemy->y, z_player.x, z_player.y),
                      false);
            } break;
        }
    }

    Z_EVERY_DS(2) {
        enemy->frame = u4((enemy->frame + 1) % z_sprite_getNumFrames(sprite));
    }

    enemy->jetFlicker = !enemy->jetFlicker;

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

void z_enemy_takeDamage(ZEnemy* Enemy, uint8_t Damage)
{
    if(Enemy->health > Damage) {
        Enemy->health = u2(Enemy->health - Damage);
    } else {
        Enemy->health = 0;
        z_effect_circles(Enemy->x, Enemy->y);
        z_screen_shake(3);
    }
}

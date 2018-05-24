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
#include "obj_enemy.h"

#include "obj_bullete.h"
#include "obj_player.h"
#include "util_collision.h"
#include "util_effects.h"
#include "util_screen.h"
#include "util_timer.h"

struct ZEnemy {
    ZPoolObjHeader poolObject;
    ZFix x, y;
    uint8_t angle : 7;
    bool jetFlicker : 1;
    uint8_t frame : 4;
    uint8_t typeId : 4;
    uint8_t flyId : 4;
    uint8_t attackId : 4;
    uint8_t flyCounter;
    uint8_t attackCounter;
    uint8_t flyState : 4;
    uint8_t attackState : 4;
    uint8_t health : 2;
};

typedef struct {
    uint8_t w : 4;
    uint8_t h : 4;
    uint8_t health : 2;
    uint8_t damage : 3;
    uint8_t speedShift : 3;
    uint8_t attackPeriodDs : 5;
    uint8_t sprite : 3;
} ZEnemyData;

Z_POOL_DECLARE(ZEnemy, 16, g_pool);

ZEnemyData g_data[Z_ENEMY_NUM];

static void enemy(uint8_t Index, uint8_t Sprite, uint8_t Width, uint8_t Height, uint8_t Health, uint8_t Damage, uint8_t SpeedShift, uint8_t AttackDs)
{
    g_data[Index].w = u4(Width);
    g_data[Index].h = u4(Height);
    g_data[Index].health = u2(Health);
    g_data[Index].damage = u3(Damage);
    g_data[Index].speedShift = u3(SpeedShift);
    g_data[Index].attackPeriodDs = u5(AttackDs);
    g_data[Index].sprite = u3(Sprite);
}

void z_enemy_setup(void)
{
    z_pool_register(Z_POOL_ENEMY, g_pool);

    enemy(Z_ENEMY_ASTEROID, Z_SPRITE_ASTEROID, 8, 8, 3, 0, 1, 0);
    enemy(Z_ENEMY_SHIP0,    Z_SPRITE_ENEMY00,  7, 5, 1, 2, 1, 20);
    enemy(Z_ENEMY_SHIP1,    Z_SPRITE_ENEMY01,  7, 5, 1, 4, 0, 15);
    enemy(Z_ENEMY_SHIP2,    Z_SPRITE_ENEMY02,  7, 6, 2, 6, 1, 15);
    enemy(Z_ENEMY_SHIP3,    Z_SPRITE_ENEMY00,  7, 5, 3, 3, 1, 10);

    z_timer_start(Z_TIMER_ENEMY_FRAME, 2);
}

void z_enemy_init(ZEnemy* Enemy, int16_t X, int16_t Y, uint8_t TypeId, uint8_t FlyId, uint8_t AttackId)
{
    Enemy->x = z_fix_fromInt(X);
    Enemy->y = z_fix_fromInt(Y);
    Enemy->angle = Z_ANGLE_270;
    Enemy->jetFlicker = false;
    Enemy->frame = 0;
    Enemy->typeId = u4(TypeId);
    Enemy->flyId = u4(FlyId);
    Enemy->attackId = u4(AttackId);
    Enemy->flyCounter = 0;
    Enemy->attackCounter = g_data[TypeId].attackPeriodDs;
    Enemy->flyState = 0;
    Enemy->attackState = 0;
    Enemy->health = g_data[TypeId].health;
}

static void shoot(ZEnemy* Enemy, uint8_t Angle, bool ExtraSpeed)
{
    ZBulletE* b = z_pool_alloc(Z_POOL_BULLETE);

    if(b) {
        z_bullete_init(b,
                       zf(Enemy->x + z_fix_fromInt(z_screen_getXShake())),
                       Enemy->y,
                       Angle,
                       ExtraSpeed,
                       g_data[Enemy->typeId].damage);
    }
}

bool z_enemy_tick(ZPoolObjHeader* Enemy, void* Context)
{
    Z_UNUSED(Context);

    ZEnemy* enemy = (ZEnemy*)Enemy;

    uint8_t type = enemy->typeId;
    uint8_t sprite = g_data[type].sprite;
    bool move = true;

    switch(enemy->flyId) {
        case Z_FLY_STILL: {
            move = false;
        } break;

        case Z_FLY_FOLLOW: {
            if(enemy->flyCounter-- == 0) {
                enemy->flyCounter = z_timer_dsToTicks(5);

                ZFix playerX, playerY;
                z_player_getCoords(&playerX, &playerY);

                int16_t eX = z_fix_toInt(enemy->x);
                int16_t pX = z_fix_toInt(playerX);

                if(eX < pX) {
                    enemy->angle = Z_ANGLE_292;
                } else if(eX > pX) {
                    enemy->angle = Z_ANGLE_247;
                } else {
                    enemy->angle = Z_ANGLE_270;
                }
            }
        } break;

        case Z_FLY_CIRCLE_ABS: {
            switch(enemy->flyState) {
                case 0: {
                    if(++enemy->flyCounter == 150) {
                        enemy->flyCounter = 0;
                        enemy->flyState = 1;
                    }
                } break;

                case 1: {
                    if(++enemy->flyCounter == 2) {
                        enemy->flyCounter = 0;
                        enemy->angle = Z_ANGLE_WRAP(enemy->angle
                                                    + Z_ANGLES_NUM / 128);
                    }
                } break;
            }
        } break;

        case Z_FLY_CIRCLE_REL: {
            switch(enemy->flyState) {
                case 0: {
                    if(z_fix_toInt(enemy->y) > Z_SCREEN_H / 2) {
                        enemy->flyState = 1;
                    }
                } break;

                case 1: {
                    if(++enemy->flyCounter == 2) {
                        enemy->flyCounter = 0;
                        enemy->angle = Z_ANGLE_WRAP(enemy->angle
                                                    + Z_ANGLES_NUM / 128);
                    }
                } break;
            }
        } break;

        case Z_FLY_SQUARE_ABS: {
            switch(enemy->flyState) {
                case 0: {
                    if(++enemy->flyCounter == 150) {
                        enemy->flyCounter = 0;
                        enemy->flyState = 1;
                    }
                } break;

                case 1: {
                    if(++enemy->flyCounter == 50) {
                        enemy->flyCounter = 0;
                        enemy->angle = Z_ANGLE_WRAP(enemy->angle + Z_ANGLE_090);
                    }
                } break;
            }
        } break;

        case Z_FLY_SQUARE_REL: {
            switch(enemy->flyState) {
                case 0: {
                    if(z_fix_toInt(enemy->y) > Z_SCREEN_H / 2) {
                        enemy->flyState = 1;
                    }
                } break;

                case 1: {
                    if(++enemy->flyCounter == 100) {
                        enemy->flyCounter = 0;
                        enemy->angle = Z_ANGLE_WRAP(enemy->angle + Z_ANGLE_090);
                    }
                } break;
            }
        } break;

        case Z_FLY_ZIGZAG: {
            switch(enemy->flyState) {
                case 0: {
                    if(++enemy->flyCounter == 40) {
                        enemy->flyCounter = 0;
                        enemy->flyState = 2;
                    }

                    enemy->angle = Z_ANGLE_337;
                } break;

                case 1: {
                    if(++enemy->flyCounter == 80) {
                        enemy->flyCounter = 0;
                        enemy->flyState = 2;
                    }

                    enemy->angle = Z_ANGLE_337;
                } break;

                case 2: {
                    if(++enemy->flyCounter == 80) {
                        enemy->flyCounter = 0;
                        enemy->flyState = 1;
                    }

                    enemy->angle = Z_ANGLE_202;
                } break;
            }
        } break;
    }

    if(move) {
        ZFix cos = z_fix_cos(enemy->angle);
        ZFix sin = z_fix_sin(enemy->angle);
        ZFix speed = g_data[type].speedShift;

        enemy->x = zf(enemy->x + (cos >> speed));
        enemy->y = zf(enemy->y - (sin >> speed));
    }

    if(enemy->y >= 0 && enemy->attackCounter-- == 0) {
        enemy->attackCounter =
            z_timer_dsToTicks(g_data[enemy->typeId].attackPeriodDs);

        switch(enemy->attackId) {
            case Z_ATTACK_FRONT: {
                shoot(enemy, Z_ANGLE_270, false);
            } break;

            case Z_ATTACK_TARGET: {
                ZFix playerX, playerY;
                z_player_getCoords(&playerX, &playerY);

                shoot(enemy,
                      z_fix_atan(enemy->x, enemy->y, playerX, playerY),
                      false);
            } break;
        }
    }

    if(z_timer_expired(Z_TIMER_ENEMY_FRAME)) {
        enemy->frame = u4((enemy->frame + 1) % z_sprite_getNumFrames(sprite));
    }

    enemy->jetFlicker = !enemy->jetFlicker;

    if(z_player_checkCollision(enemy->x,
                               enemy->y,
                               g_data[type].w,
                               g_data[type].h,
                               Z_PLAYER_MAX_SHIELD)) {

        z_enemy_takeDamage(enemy, UINT8_MAX);
        z_player_scorePoints(Z_POINTS_ENEMY_SMASHED);
    }

    return enemy->health > 0
        && z_fix_toInt(enemy->y) - z_sprite_getHeight(sprite) / 2 < Z_SCREEN_H;
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

void z_enemy_draw(ZPoolObjHeader* Enemy)
{
    ZEnemy* enemy = (ZEnemy*)Enemy;
    int16_t x = z_fix_toInt(enemy->x);
    int16_t y = z_fix_toInt(enemy->y);

    if(enemy->jetFlicker) {
        drawJets(enemy->typeId, x, y);
    }

    z_sprite_blitCentered(g_data[enemy->typeId].sprite,
                          i16(x + z_screen_getXShake()),
                          i16(y + z_screen_getYShake()),
                          enemy->frame);
}

void z_enemy_takeDamage(ZEnemy* Enemy, uint8_t Damage)
{
    if(Enemy->health > Damage) {
        Enemy->health = u2(Enemy->health - Damage);
        z_sfx_play(Z_SFX_ENEMY_HURT);
    } else {
        Enemy->health = 0;
        z_sfx_play(Z_SFX_ENEMY_DIE);
        z_effect_circles(Enemy->x, Enemy->y);
        z_screen_shake(3);
    }
}

void z_enemy_getSize(ZEnemyId Enemy, int16_t* Width, int16_t* Height)
{
    *Width = z_sprite_getWidth(g_data[Enemy].sprite);
    *Height = z_sprite_getHeight(g_data[Enemy].sprite);
}

typedef struct {
    int16_t x, y;
    int8_t w, h;
    uint8_t damage : 3;
    bool hit : 1;
} ZCollisionContext;

static bool enemyShipCollision(ZPoolObjHeader* Enemy, void* Context)
{
    ZCollisionContext* context = Context;

    if(context->hit) {
        return true;
    }

    ZEnemy* enemy = (ZEnemy*)Enemy;

    if(z_collision_boxAndBox(context->x,
                             context->y,
                             context->w,
                             context->h,
                             z_fix_toInt(enemy->x),
                             z_fix_toInt(enemy->y),
                             g_data[enemy->typeId].w,
                             g_data[enemy->typeId].h)) {

        context->hit = true;
        z_effect_particles(enemy->x, enemy->y, 4);
        z_enemy_takeDamage(enemy, context->damage);

        if(enemy->health > 0) {
            z_player_scorePoints(Z_POINTS_ENEMY_HIT);
        } else {
            z_player_scorePoints(Z_POINTS_ENEMY_DESTROYED);
        }
    }

    return enemy->health > 0;
}

bool z_enemy_checkCollisions(ZFix X, ZFix Y, int8_t W, int8_t H, uint8_t Damage)
{
    ZCollisionContext context = {
        z_fix_toInt(X), z_fix_toInt(Y), W, H, u3(Damage), false
    };

    z_pool_tick(Z_POOL_ENEMY, enemyShipCollision, &context);

    return context.hit;
}

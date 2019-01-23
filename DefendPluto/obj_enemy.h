/*
    Copyright 2017-2018 Alex Margarit <alex@alxm.org>
    This file is part of Defend Pluto, a video game.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "util_fix.h"
#include "util_pool.h"

typedef enum Z_ENUM_PACK {
    Z_ENEMY_INVALID = -1,
    Z_ENEMY_ASTEROID,
    Z_ENEMY_SHIP0,
    Z_ENEMY_SHIP1,
    Z_ENEMY_SHIP2,
    Z_ENEMY_SHIP3,
    Z_ENEMY_NUM
} ZEnemyId;

typedef enum Z_ENUM_PACK {
    Z_FLY_INVALID = -1,
    Z_FLY_STILL,
    Z_FLY_DOWN,
    Z_FLY_FOLLOW,
    Z_FLY_CIRCLE_ABS,
    Z_FLY_CIRCLE_REL,
    Z_FLY_SQUARE_ABS,
    Z_FLY_SQUARE_REL,
    Z_FLY_ZIGZAG,
    Z_FLY_NUM
} ZFlyId;

typedef enum Z_ENUM_PACK {
    Z_ATTACK_INVALID = -1,
    Z_ATTACK_NONE,
    Z_ATTACK_FRONT,
    Z_ATTACK_TARGET,
    Z_ATTACK_NUM
} ZAttackId;

typedef struct ZEnemy ZEnemy;

extern void z_enemy_setup(void);

extern void z_enemy_init(ZEnemy* Enemy, int16_t X, int16_t Y, uint8_t TypeId, uint8_t FlyId, uint8_t AttackId);
extern ZPoolTick z_enemy_tick;
extern ZPoolDraw z_enemy_draw;

extern void z_enemy_getSize(ZEnemyId Enemy, int16_t* Width, int16_t* Height);
extern void z_enemy_takeDamage(ZEnemy* Enemy, uint8_t Damage);
extern bool z_enemy_checkCollisions(ZFix X, ZFix Y, int8_t W, int8_t H, uint8_t Damage);

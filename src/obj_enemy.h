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

typedef enum {
    Z_ENEMY_INVALID = -1,
    Z_ENEMY_ASTEROID,
    Z_ENEMY_SHIP0,
    Z_ENEMY_SHIP1,
    Z_ENEMY_SHIP2,
    Z_ENEMY_NUM
} ZEnemyId;

typedef enum {
    Z_FLY_INVALID = -1,
    Z_FLY_STILL,
    Z_FLY_DOWN,
    Z_FLY_FOLLOW,
    Z_FLY_NUM
} ZFlyId;

typedef enum {
    Z_ATTACK_INVALID = -1,
    Z_ATTACK_NONE,
    Z_ATTACK_FRONT,
    Z_ATTACK_TARGET,
    Z_ATTACK_NUM
} ZAttackId;

#define Z_ENEMY_MAX_HEALTH 3

typedef struct {
    ZPoolObject poolObject;
    ZFix x, y;
    uint8_t angle : 7;
    bool jetFlicker : 1;
    uint8_t frame : 4;
    uint8_t typeId : 4;
    uint8_t flyId : 4;
    uint8_t attackId : 4;
    uint8_t flyCounter;
    uint8_t attackCounter;
    uint8_t health : 2;
} ZEnemy;

typedef struct {
    uint8_t w : 4;
    uint8_t h : 4;
    uint8_t health : 2;
    uint8_t damage : 3;
    uint8_t speedShift : 3;
    uint8_t attackPeriodDs : 5;
    uint8_t sprite : 3;
} ZEnemyData;

extern ZEnemyData z_enemy_data[Z_ENEMY_NUM];

extern void z_enemy_setup(void);

extern void z_enemy_init(ZEnemy* Enemy, int16_t X, int16_t Y, uint8_t TypeId, uint8_t FlyId, uint8_t AttackId);
extern ZPoolTickCallback z_enemy_tick;
extern ZPoolDrawCallback z_enemy_draw;

extern void z_enemy_takeDamage(ZEnemy* Enemy, uint8_t Damage);

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

typedef enum {
    Z_FLY_INVALID = -1,
    Z_FLY_LINE,
    Z_FLY_ZIGZAG,
    Z_FLY_CURVE,
    Z_FLY_NUM
} ZFlyId;

typedef enum {
    Z_ATTACK_INVALID = -1,
    Z_ATTACK_NONE,
    Z_ATTACK_STRAIGHT,
    Z_ATTACK_TARGET,
    Z_ATTACK_NUM
} ZAttackId;

typedef struct {
    ZPoolObject poolObject;
    ZFix x, y;
    uint8_t angle : 7;
    bool jetFlicker : 1;
    uint8_t typeId : 4;
    uint8_t frame : 4;
    struct {
        uint8_t state : 4;
        uint8_t flags : 4;
    } ai;
    struct {
        uint8_t id : 4;
        uint8_t state : 4;
        uint8_t flipX : 1;
        uint8_t flipY : 1;
        uint8_t counter : 6;
    } fly;
    struct {
        uint8_t id : 4;
        uint8_t counter : 4;
    } attack;
} ZEnemy;

typedef void (*ZEnemyCallback)(ZEnemy*);

typedef struct {
    ZEnemyCallback callback;
    uint8_t counterMax : 6;
} ZEnemyFlyPattern;

#define Z_ENEMY_POOL_NUM 16

extern ZEnemyCallback z_enemy_aiTable[Z_ENEMY_NUM];
extern ZEnemyFlyPattern z_enemy_flyTable[Z_FLY_NUM];
extern ZEnemyCallback z_enemy_attackTable[Z_ATTACK_NUM];

extern void z_enemy_init(ZEnemy* Enemy, int8_t X, int8_t Y, uint8_t TypeId, uint8_t AiState, uint8_t AiFlags, uint8_t DropId);
extern bool z_enemy_tick(ZPoolObject* Enemy);
extern void z_enemy_draw(ZPoolObject* Enemy);

extern bool z_enemy_checkCollisions(int8_t X, int8_t Y, int8_t W, int8_t H, bool AllowMultipleCollisions);

extern void z_enemy_setFly(ZEnemy* Enemy, uint8_t FlyId);
extern void z_enemy_setAttack(ZEnemy* Enemy, uint8_t AttackId);

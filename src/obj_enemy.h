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
    Z_ENEMY_INVALID = -1,
    Z_ENEMY_ASTEROID,
    Z_ENEMY_SHIP0,
    Z_ENEMY_SHIP1,
    Z_ENEMY_SHIP2,
    Z_ENEMY_NUM
} ZEnemyId;

typedef enum {
    Z_ATTACK_INVALID = -1,
    Z_ATTACK_NONE,
    Z_ATTACK_STRAIGHT,
    Z_ATTACK_TARGET,
    Z_ATTACK_NUM
} ZAttackId;

#define Z_ENEMY_MAX_HEALTH 3

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
        uint8_t state : 4;
        uint8_t counter;
    } fly;
    struct {
        uint8_t counter;
    } attack;
    uint8_t health;
} ZEnemy;

typedef void (ZEnemyCallback)(ZEnemy*);

typedef struct {
    uint8_t sprite;
    ZEnemyCallback* ai;
    uint8_t w : 4;
    uint8_t h : 4;
    uint8_t health : 2;
    uint8_t damage : 3;
    uint8_t speedShift : 3;
} ZEnemyData;

#define Z_AI switch(Enemy->ai.state)
#define Z_AI_DONE() Enemy->ai.state = 0xf;
#define Z_AI_GO(State) Enemy->ai.state = (State);
#define Z_AI_FLAG(Bit) if(Enemy->ai.flags & (1 << (Bit)))

#define Z_AI_STATE(State)                 \
    case (State):                         \
        for(uint8_t z__u = 0; ; z__u = 1) \
            if(z__u == 1) {               \
                return;                   \
            } else

#define Z_FLY switch(Enemy->fly.state)
#define Z_FLY_GO(State) Enemy->fly.state = (State);
#define Z_FLY_STATE(State) Z_AI_STATE(State)

#define Z_FLY_COUNTER_SET(Ds)                    \
    Enemy->fly.counter = Z_DS_TO_FRAMES(Ds) / 2;

#define Z_FLY_EVERY_DS(Ds)                               \
    for(uint8_t z__u = 0; z__u < 2; z__u++)              \
        if(z__u == 0) {                                  \
            if(Enemy->fly.counter-- == 0) {              \
                Enemy->fly.counter = Z_DS_TO_FRAMES(Ds); \
            } else {                                     \
                break;                                   \
            }                                            \
        } else

extern ZEnemyData z_enemy_data[Z_ENEMY_NUM];

extern void z_enemy_setup(void);

extern void z_enemy_init(ZEnemy* Enemy, int16_t X, int16_t Y, uint8_t TypeId, uint8_t AiState, uint8_t AiFlags);
extern ZPoolTickCallback z_enemy_tick;
extern ZPoolDrawCallback z_enemy_draw;

extern bool z_enemy_checkCollisions(int16_t X, int16_t Y, int8_t W, int8_t H, uint8_t Damage, bool AllowMultipleCollisions);

extern void z_enemy_attack(ZEnemy* Enemy, uint8_t AttackId);

extern ZEnemyCallback z_enemy_ai_asteroid;
extern ZEnemyCallback z_enemy_ai_ship0;
extern ZEnemyCallback z_enemy_ai_ship1;
extern ZEnemyCallback z_enemy_ai_ship2;

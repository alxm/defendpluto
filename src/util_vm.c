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
#include "util_fix.h"
#include "util_pool.h"
#include "util_vm.h"
#include "obj_enemy.h"
#include "data_levels.h"

typedef enum {
    Z_OP_SPAWN,
    Z_OP_WAIT,
    Z_OP_WAITCLEAR,
    Z_OP_LOOP,
    Z_OP_END,
    Z_OP_OVER,
    Z_OP_NUM
} ZOpType;

typedef struct {
    bool (*callback)(void);
    uint8_t bytes;
} ZOp;

static uint16_t g_pc;
static uint16_t g_loopStart;
static uint8_t g_loopCounter;
static uint8_t g_wait;
static bool g_reset = true;
static ZOp g_ops[Z_OP_NUM];

#define Z_READ(Offset) Z_PGM_READ_UINT8(z_data_levels[g_pc + Offset])

static bool handle_spawn(void)
{
    /*
     * 8b    8b      8b      4b        4b      8b
     * spawn x_coord y_coord sprite_id ai_id   ai_args
     * spawn 64      -8      enemy0    nobrain 0
     */
    int8_t x = (int8_t)Z_READ(1);
    int8_t y = (int8_t)Z_READ(2);
    uint8_t sprite_id = Z_READ(3) >> 4;
    uint8_t ai_id = Z_READ(3) & 0xf;
    uint8_t ai_args = Z_READ(4);

    ZEnemy* e = z_pool_alloc(Z_POOL_ENEMY);

    if(e == NULL) {
        return false;
    }

    z_enemy_init(e, x, y, sprite_id, ai_id, ai_args);

    return true;
}

static bool handle_wait(void)
{
    /*
     * 8b   8b
     * wait frames
     * wait 30
     */
    g_wait = Z_READ(1);

    return true;
}

static bool handle_waitclear(void)
{
    /*
     * 8b
     * waitclear
     * waitclear
     */
    return z_pool_getNumActive(Z_POOL_ENEMY) == 0;
}

static bool handle_loop(void)
{
    /*
     * 8b   8b
     * loop num_times
     * loop 10
     */
    uint8_t num_times = Z_READ(1);

    if(num_times == 0) {
        do {
            g_pc = (uint16_t)(g_pc + g_ops[Z_READ(0)].bytes);
        } while(Z_READ(0) != Z_OP_END);
    } else {
        g_loopStart = (uint16_t)(g_pc + g_ops[Z_OP_LOOP].bytes);
        g_loopCounter = num_times;
    }

    return true;
}

static bool handle_end(void)
{
    /*
     * 8b
     * end
     * end
     */
    if(--g_loopCounter) {
        g_pc = g_loopStart;
        return false;
    }

    return true;
}

static bool handle_over(void)
{
    /*
     * 8b
     * over
     * over
     */
    g_reset = true;

    return false;
}

void z_vm_setup(void)
{
    g_ops[Z_OP_SPAWN] = (ZOp){handle_spawn, 5};
    g_ops[Z_OP_WAIT] = (ZOp){handle_wait, 2};
    g_ops[Z_OP_WAITCLEAR] = (ZOp){handle_waitclear, 1};
    g_ops[Z_OP_LOOP] = (ZOp){handle_loop, 2};
    g_ops[Z_OP_END] = (ZOp){handle_end, 1};
    g_ops[Z_OP_OVER] = (ZOp){handle_over, 1};
}

void z_vm_tick(void)
{
    if(g_reset) {
        g_pc = 0;
        g_wait = 0;
        g_reset = false;
    }

    if(g_wait) {
        g_wait--;
        return;
    }

    uint8_t instruction = Z_READ(0);

    if(g_ops[instruction].callback()) {
        g_pc = (uint16_t)(g_pc + g_ops[instruction].bytes);
    }
}

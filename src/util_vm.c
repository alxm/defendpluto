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
    Z_OP_INVALID = -1,
    Z_OP_SPAWN,
    Z_OP_WAIT,
    Z_OP_WAITCLEAR,
    Z_OP_LOOP,
    Z_OP_END,
    Z_OP_OVER,
    Z_OP_SET,
    Z_OP_INC,
    Z_OP_NUM
} ZOpType;

typedef enum {
    Z_VAR_INVALID = -1,
    Z_VAR_X,
    Z_VAR_Y,
    Z_VAR_NUM
} ZVar;

typedef struct {
    bool (*callback)(uint8_t Flags);
    uint8_t bytes;
} ZOp;

static uint16_t g_pc;
static uint16_t g_loopStart;
static uint8_t g_loopCounter;
static uint8_t g_wait;
static int8_t g_vars[Z_VAR_NUM];
static ZOp g_ops[Z_OP_NUM];

#define Z_READ(Offset) (Z_PGM_READ_UINT8(z_data_levels[g_pc + Offset]))
#define Z_READ_OP()    (Z_READ(0) >> 4)
#define Z_READ_FLAGS() (Z_READ(0) & 0xf)

#define Z_EVAL_VAR(ArgIndex, CVar)      \
    if(Flags & (1 << (ArgIndex - 1))) { \
        CVar = g_vars[CVar];            \
    }

static bool handle_spawn(uint8_t Flags)
{
    Z_UNUSED(Flags);

    /*
     * 8b    8b      8b      4b      4b      8b
     * spawn x_coord y_coord type_id ai_id   ai_args
     * spawn 64      -8      enemy0  nobrain 0
     */
    int8_t x = (int8_t)Z_READ(1);
    int8_t y = (int8_t)Z_READ(2);
    uint8_t type_id = Z_READ(3) >> 4;
    uint8_t ai_id = Z_READ(3) & 0xf;
    uint8_t ai_args = Z_READ(4);

    Z_EVAL_VAR(1, x);
    Z_EVAL_VAR(2, y);

    ZEnemy* e = z_pool_alloc(Z_POOL_ENEMY);

    if(e == NULL) {
        return false;
    }

    z_enemy_init(e, x, y, type_id, ai_id, ai_args);

    return true;
}

static bool handle_wait(uint8_t Flags)
{
    Z_UNUSED(Flags);

    /*
     * 8b   8b
     * wait frames
     * wait 30
     */
    g_wait = Z_READ(1);

    return true;
}

static bool handle_waitclear(uint8_t Flags)
{
    Z_UNUSED(Flags);

    /*
     * 8b
     * waitclear
     * waitclear
     */
    return z_pool_noActive(Z_POOL_ENEMY);
}

static bool handle_loop(uint8_t Flags)
{
    Z_UNUSED(Flags);

    /*
     * 8b   8b
     * loop num_times
     * loop 10
     */
    uint8_t num_times = Z_READ(1);

    if(num_times == 0) {
        uint8_t op;

        do {
            op = Z_READ_OP();
            g_pc = (uint16_t)(g_pc + g_ops[op].bytes);
        } while(op != Z_OP_END);

        return false;
    } else {
        g_loopStart = (uint16_t)(g_pc + g_ops[Z_OP_LOOP].bytes);
        g_loopCounter = num_times;
    }

    return true;
}

static bool handle_end(uint8_t Flags)
{
    Z_UNUSED(Flags);

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

static bool handle_over(uint8_t Flags)
{
    Z_UNUSED(Flags);

    /*
     * 8b
     * over
     * over
     */
    z_vm_reset();

    return false;
}

static bool handle_set(uint8_t Flags)
{
    Z_UNUSED(Flags);

    /*
     * 8b  8b     8b
     * set var_id value
     * set x      32
     */
    uint8_t var_id = Z_READ(1);
    int8_t value = (int8_t)Z_READ(2);

    g_vars[var_id] = value;

    return true;
}


static bool handle_inc(uint8_t Flags)
{
    Z_UNUSED(Flags);

    /*
     * 8b  8b     8b
     * inc var_id value
     * inc x      16
     */
    uint8_t var_id = Z_READ(1);
    int8_t value = (int8_t)Z_READ(2);

    g_vars[var_id] = (int8_t)(g_vars[var_id] + value);

    return true;
}

void z_vm_setup(void)
{
    g_ops[Z_OP_SPAWN] = (ZOp){handle_spawn, 5};
    g_ops[Z_OP_WAIT] = (ZOp){handle_wait, 2};
    g_ops[Z_OP_WAITCLEAR] = (ZOp){handle_waitclear, 1};
    g_ops[Z_OP_LOOP] = (ZOp){handle_loop, 2};
    g_ops[Z_OP_END] = (ZOp){handle_end, 1};
    g_ops[Z_OP_OVER] = (ZOp){handle_over, 1};
    g_ops[Z_OP_SET] = (ZOp){handle_set, 3};
    g_ops[Z_OP_INC] = (ZOp){handle_inc, 3};

    z_vm_reset();
}

void z_vm_reset(void)
{
    g_pc = 0;
    g_wait = 0;

    g_vars[Z_VAR_X] = 0;
    g_vars[Z_VAR_Y] = 0;
}

void z_vm_tick(void)
{
    if(g_wait) {
        g_wait--;
        return;
    }

    uint8_t op = Z_READ_OP();
    uint8_t flags = Z_READ_FLAGS();

    if(g_ops[op].callback(flags)) {
        g_pc = (uint16_t)(g_pc + g_ops[op].bytes);
    }
}

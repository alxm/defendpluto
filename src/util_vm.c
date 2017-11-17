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
#include "util_enemy.h"
#include "util_fix.h"
#include "util_pool.h"
#include "util_vm.h"
#include "obj_enemy.h"
#include "data_levels.h"

typedef enum {
    Z_OP_INVALID = -1,
    Z_OP_OVER,
    Z_OP_SET,
    Z_OP_INC,
    Z_OP_FLIP,
    Z_OP_LOOP,
    Z_OP_END,
    Z_OP_WAIT,
    Z_OP_WAITCLEAR,
    Z_OP_SPAWN,
    Z_OP_NUM
} ZOpType;

typedef struct {
    bool (*callback)(uint8_t Flags);
    uint8_t bytes;
} ZOp;

static ZOp g_ops[Z_OP_NUM];

static struct {
    uint16_t pc;
    uint8_t loopIndex;
    struct {
        uint16_t start;
        uint8_t counter;
    } loopStack[Z_LEVELS_NESTED_LOOPS_MAX];
    uint8_t block;
    int8_t vars[Z_LEVELS_VARS_NUM];
} g_vm;

#define Z__READ(Offset) Z_PGM_READ_UINT8(z_data_levels[g_vm.pc + Offset])

#define Z__CHECKVAR(Type, CVar, ArgIndex) \
    if(Flags & (1 << ArgIndex)) {         \
        CVar = (Type)g_vm.vars[CVar];     \
    }

#define Z_READ_OP()           Z__READ(0)
#define Z_READ_FLAGS()        Z__READ(1)
#define Z_READ_ARG(ByteIndex) Z__READ(2 + ByteIndex)

#define Z_READ_ARGI8(CVar, ArgIndex, ByteIndex) \
    CVar = i8(Z_READ_ARG(ByteIndex));           \
    Z__CHECKVAR(int8_t, CVar, ArgIndex)

#define Z_READ_ARGU8(CVar, ArgIndex, ByteIndex) \
    CVar = u8(Z_READ_ARG(ByteIndex));           \
    Z__CHECKVAR(uint8_t, CVar, ArgIndex)

#define Z_READ_ARGU4H(CVar, ArgIndex, ByteIndex) \
    CVar = Z_READ_ARG(ByteIndex) >> 4;           \
    Z__CHECKVAR(uint8_t, CVar, ArgIndex)

#define Z_READ_ARGU4L(CVar, ArgIndex, ByteIndex) \
    CVar = Z_READ_ARG(ByteIndex) & 0xf;          \
    Z__CHECKVAR(uint8_t, CVar, ArgIndex)

static bool op_over(uint8_t Flags)
{
    Z_UNUSED(Flags);

    /*
     * 8b   8b
     * over flags
     * over
     */
    z_vm_reset();

    return false;
}

static bool op_set(uint8_t Flags)
{
    Z_UNUSED(Flags);

    /*
     * 8b  8b    8b     8b
     * set flags var_id value
     * set       x      32
     */
    uint8_t var_id;
    int8_t value;
    Z_READ_ARGU8(var_id, 0, 0);
    Z_READ_ARGI8(value, 1, 1);

    g_vm.vars[var_id] = value;

    return true;
}

static bool op_inc(uint8_t Flags)
{
    Z_UNUSED(Flags);

    /*
     * 8b  8b    8b     8b
     * inc flags var_id value
     * inc       x      16
     */
    uint8_t var_id;
    int8_t value;
    Z_READ_ARGU8(var_id, 0, 0);
    Z_READ_ARGI8(value, 1, 1);

    g_vm.vars[var_id] = i8(g_vm.vars[var_id] + value);

    return true;
}

static bool op_flip(uint8_t Flags)
{
    Z_UNUSED(Flags);

    /*
     * 8b   8b    8b
     * flip flags var_id
     * flip       x
     */
    uint8_t var_id;
    Z_READ_ARGU8(var_id, 0, 0);

    g_vm.vars[var_id] ^= 1;

    return true;
}

static bool op_loop(uint8_t Flags)
{
    Z_UNUSED(Flags);

    /*
     * 8b   8b    8b
     * loop flags num_times
     * loop       10
     */
    uint8_t num_times;
    Z_READ_ARGU8(num_times, 0, 0);

    if(num_times == 0 || g_vm.loopIndex == 0) {
        uint8_t op;

        do {
            op = Z_READ_OP();
            g_vm.pc = u16(g_vm.pc + g_ops[op].bytes);
        } while(op != Z_OP_END);

        return false;
    } else {
        g_vm.loopIndex--;
        g_vm.loopStack[g_vm.loopIndex].start =
                                        u16(g_vm.pc + g_ops[Z_OP_LOOP].bytes);
        g_vm.loopStack[g_vm.loopIndex].counter = num_times;
    }

    return true;
}

static bool op_end(uint8_t Flags)
{
    Z_UNUSED(Flags);

    /*
     * 8b  8b
     * end flags
     * end
     */
    if(--g_vm.loopStack[g_vm.loopIndex].counter) {
        g_vm.pc = g_vm.loopStack[g_vm.loopIndex].start;
        return false;
    }

    g_vm.loopIndex++;

    return true;
}

static bool op_wait(uint8_t Flags)
{
    Z_UNUSED(Flags);

    /*
     * 8b   8b    8b
     * wait flags frames
     * wait       30
     */
    Z_READ_ARGU8(g_vm.block, 0, 0);

    return true;
}

static bool op_waitclear(uint8_t Flags)
{
    Z_UNUSED(Flags);

    /*
     * 8b        8b
     * waitclear flags
     * waitclear
     */
    return z_pool_noActive(Z_POOL_ENEMY);
}

static bool op_spawn(uint8_t Flags)
{
    Z_UNUSED(Flags);

    /*
     * 8b    8b    8b      8b      4b      4b
     * spawn flags x_coord y_coord type_id drop_id
     * spawn       64      -8      enemy0  powerup
     */
    int8_t x, y;
    uint8_t type_id, drop_id;
    Z_READ_ARGI8(x, 0, 0);
    Z_READ_ARGI8(y, 1, 1);
    Z_READ_ARGU4H(type_id, 2, 2);
    Z_READ_ARGU4L(drop_id, 3, 2);

    ZEnemy* e = z_pool_alloc(Z_POOL_ENEMY);

    if(e == NULL) {
        return false;
    }

    z_enemy_init(e, x, y, type_id, drop_id);

    return true;
}

void z_vm_setup(void)
{
    #define setOp(Index, Function, ArgBytes)                                   \
        g_ops[Index] = (ZOp){Function, 1 + (ArgBytes > 0 ? 1 + ArgBytes : 0)};

    setOp(Z_OP_OVER, op_over, 0);
    setOp(Z_OP_SET, op_set, 2);
    setOp(Z_OP_INC, op_inc, 2);
    setOp(Z_OP_FLIP, op_flip, 1);
    setOp(Z_OP_LOOP, op_loop, 1);
    setOp(Z_OP_END, op_end, 0);
    setOp(Z_OP_WAIT, op_wait, 1);
    setOp(Z_OP_WAITCLEAR, op_waitclear, 0);
    setOp(Z_OP_SPAWN, op_spawn, 3);

    z_vm_reset();
}

void z_vm_reset(void)
{
    g_vm.pc = 0;
    g_vm.block = 0;
    g_vm.loopIndex = Z_LEVELS_NESTED_LOOPS_MAX;

    for(uint8_t v = Z_LEVELS_VARS_NUM; v--; ) {
        g_vm.vars[v] = 0;
    }
}

void z_vm_tick(void)
{
    if(g_vm.block) {
        g_vm.block--;
        return;
    }

    uint8_t op = Z_READ_OP();
    uint8_t flags = 0;

    if(g_ops[op].bytes > 1) {
        flags = Z_READ_FLAGS();
    }

    if(g_ops[op].callback(flags)) {
        g_vm.pc = u16(g_vm.pc + g_ops[op].bytes);
    }
}

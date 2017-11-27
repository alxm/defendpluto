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

#include "platform.h"
#include "util_enemy.h"
#include "util_fix.h"
#include "util_fps.h"
#include "util_graphics.h"
#include "util_pool.h"
#include "util_screen.h"
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
    Z_OP_ITER,
    Z_OP_ENDI,
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
        uint8_t counterMax;
    } loopStack[Z_LEVELS_NESTED_LOOPS_MAX];
    uint8_t waitCounter;
    int8_t vars[Z_LEVELS_VARS_NUM];
} g_vm;

#define Z__READ(Offset) Z_PGM_READ_UINT8(z_data_levels + g_vm.pc + Offset)

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
     * 8b
     * over
     * over
     */
    z_vm_reset();

    return false;
}

static bool op_set(uint8_t Flags)
{
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
    /*
     * 8b   8b    8b
     * loop flags num_times
     * loop       10
     */
    uint8_t num_times;
    Z_READ_ARGU8(num_times, 0, 0);

    if(num_times == 0) {
        for(uint8_t op = Z_OP_LOOP, loopCount = 1; loopCount > 0; ) {
            g_vm.pc = u16(g_vm.pc + g_ops[op].bytes);
            op = Z_READ_OP();

            if(op == Z_OP_LOOP) {
                loopCount++;
            } else if(op == Z_OP_END) {
                loopCount--;
            }
        }
    } else {
        g_vm.loopIndex--;
        g_vm.loopStack[g_vm.loopIndex].start = g_vm.pc;
        g_vm.loopStack[g_vm.loopIndex].counter = 0;
        g_vm.loopStack[g_vm.loopIndex].counterMax = num_times;
    }

    return true;
}

static bool op_end(uint8_t Flags)
{
    Z_UNUSED(Flags);

    /*
     * 8b
     * end
     * end
     */
    if(++g_vm.loopStack[g_vm.loopIndex].counter
        == g_vm.loopStack[g_vm.loopIndex].counterMax) {

        g_vm.loopIndex++;
    } else {
        g_vm.pc = g_vm.loopStack[g_vm.loopIndex].start;
    }

    return true;
}

static bool op_iter(uint8_t Flags)
{
    /*
     * 8b   8b    8b
     * iter flags iteration
     * iter       0
     */
    uint8_t iteration;
    Z_READ_ARGU8(iteration, 0, 0);

    if(g_vm.loopStack[g_vm.loopIndex].counter != iteration) {
        for(uint8_t op = Z_OP_ITER; op != Z_OP_ENDI; op = Z_READ_OP()) {
            g_vm.pc = u16(g_vm.pc + g_ops[op].bytes);
        }
    }

    return true;
}

static bool op_endi(uint8_t Flags)
{
    Z_UNUSED(Flags);

    /*
     * 8b
     * endi
     * endi
     */
    return true;
}

static bool op_wait(uint8_t Flags)
{
    /*
     * 8b   8b    8b
     * wait flags ds
     * wait       25
     */
    if(g_vm.waitCounter > 0) {
        return --g_vm.waitCounter == 0;
    }

    uint8_t ds;
    Z_READ_ARGU8(ds, 0, 0);

    g_vm.waitCounter = Z_DS_TO_FRAMES(ds);

    return ds == 0;
}

static bool op_waitclear(uint8_t Flags)
{
    Z_UNUSED(Flags);

    /*
     * 8b
     * waitclear
     * waitclear
     */
    return z_pool_noActive(Z_POOL_ENEMY);
}

static bool op_spawn(uint8_t Flags)
{
    /*
     * 8b    8b    8b      8b      4b      4b       8b
     * spawn flags x_coord y_coord type_id ai_state ai_flags
     * spawn       64      -8      enemy0  1        0
     */
    int16_t x, y;
    uint8_t type_id, ai_state, ai_flags;
    Z_READ_ARGI8(x, 0, 0);
    Z_READ_ARGI8(y, 1, 1);
    Z_READ_ARGU4H(type_id, 2, 2);
    Z_READ_ARGU4L(ai_state, 3, 2);
    Z_READ_ARGU8(ai_flags, 4, 3);

    ZEnemy* e = z_pool_alloc(Z_POOL_ENEMY);

    if(e == NULL) {
        return false;
    }

    x = i16((Z_WIDTH - 1) * z_int16_clamp(x, 0, 100) / 100);

    if(y < 0) {
        y = i16(-z_sprite_getHeight(&z_enemyData[type_id].sprite) / 2);
    } else if(y > 100) {
        y = i16(Z_HEIGHT + z_sprite_getHeight(&z_enemyData[type_id].sprite) / 2);
    } else {
        y = i16((Z_HEIGHT - 1) * y / 100);
    }

    z_enemy_init(e, x, y, type_id, ai_state, ai_flags & 0xf);

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
    setOp(Z_OP_ITER, op_iter, 1);
    setOp(Z_OP_ENDI, op_endi, 0);
    setOp(Z_OP_WAIT, op_wait, 1);
    setOp(Z_OP_WAITCLEAR, op_waitclear, 0);
    setOp(Z_OP_SPAWN, op_spawn, 4);

    z_vm_reset();
}

void z_vm_reset(void)
{
    g_vm.pc = 0;
    g_vm.waitCounter = 0;
    g_vm.loopIndex = Z_LEVELS_NESTED_LOOPS_MAX;

    for(uint8_t v = Z_LEVELS_VARS_NUM; v--; ) {
        g_vm.vars[v] = 0;
    }
}

void z_vm_tick(void)
{
    while(true) {
        uint8_t op = Z_READ_OP();
        uint8_t flags = 0;

        if(g_ops[op].bytes > 1) {
            flags = Z_READ_FLAGS();
        }

        if(g_ops[op].callback(flags)) {
            op = Z_READ_OP();
            g_vm.pc = u16(g_vm.pc + g_ops[op].bytes);
        } else {
            break;
        }
    }
}

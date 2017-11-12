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

#define Z_VARS_NUM 2

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

typedef struct {
    bool (*callback)(uint8_t Flags);
    uint8_t bytes;
} ZOp;

static uint16_t g_pc;
static uint16_t g_loopStart;
static uint8_t g_loopCounter;
static uint8_t g_wait;
static int8_t g_vars[Z_VARS_NUM];
static ZOp g_ops[Z_OP_NUM];

#define Z__READ(Offset) Z_PGM_READ_UINT8(z_data_levels[g_pc + Offset])

#define Z__CHECKVAR(Type, CVar, ArgIndex) \
    if(Flags & (1 << ArgIndex)) {         \
        CVar = (Type)g_vars[CVar];        \
    }

#define Z_READ_FLAGS() Z__READ(0)
#define Z_READ_OP()    Z__READ(1)

#define Z_READ_ARGI8(CVar, ArgIndex, ByteIndex) \
    CVar = i8(Z__READ(2 + ByteIndex));          \
    Z__CHECKVAR(int8_t, CVar, ArgIndex)

#define Z_READ_ARGU8(CVar, ArgIndex, ByteIndex) \
    CVar = u8(Z__READ(2 + ByteIndex));          \
    Z__CHECKVAR(uint8_t, CVar, ArgIndex)

#define Z_READ_ARGU4H(CVar, ArgIndex, ByteIndex) \
    CVar = Z__READ(2 + ByteIndex) >> 4;          \
    Z__CHECKVAR(uint8_t, CVar, ArgIndex)

#define Z_READ_ARGU4L(CVar, ArgIndex, ByteIndex) \
    CVar = Z__READ(2 + ByteIndex) & 0xf;         \
    Z__CHECKVAR(uint8_t, CVar, ArgIndex)

static bool handle_spawn(uint8_t Flags)
{
    Z_UNUSED(Flags);

    /*
     * 8b    8b    8b      8b      4b      4b     8b
     * flags spawn x_coord y_coord type_id ai_id  ai_args
     *       spawn 64      -8      enemy0  zigzag 0
     */
    int8_t x, y;
    uint8_t type_id, ai_id, ai_args;

    Z_READ_ARGI8(x, 0, 0);
    Z_READ_ARGI8(y, 1, 1);
    Z_READ_ARGU4H(type_id, 2, 2);
    Z_READ_ARGU4L(ai_id, 3, 2);
    Z_READ_ARGU8(ai_args, 4, 3);

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
     * 8b    8b   8b
     * flags wait frames
     *       wait 30
     */
    Z_READ_ARGU8(g_wait, 0, 0);

    return true;
}

static bool handle_waitclear(uint8_t Flags)
{
    Z_UNUSED(Flags);

    /*
     * 8b    8b
     * flags waitclear
     *       waitclear
     */
    return z_pool_noActive(Z_POOL_ENEMY);
}

static bool handle_loop(uint8_t Flags)
{
    Z_UNUSED(Flags);

    /*
     * 8b    8b   8b
     * flags loop num_times
     *       loop 10
     */
    uint8_t num_times;

    Z_READ_ARGU8(num_times, 0, 0);

    if(num_times == 0) {
        uint8_t op;

        do {
            op = Z_READ_OP();
            g_pc = u16(g_pc + g_ops[op].bytes);
        } while(op != Z_OP_END);

        return false;
    } else {
        g_loopStart = u16(g_pc + g_ops[Z_OP_LOOP].bytes);
        g_loopCounter = num_times;
    }

    return true;
}

static bool handle_end(uint8_t Flags)
{
    Z_UNUSED(Flags);

    /*
     * 8b    8b
     * flags end
     *       end
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
     * 8b    8b
     * flags over
     *       over
     */
    z_vm_reset();

    return false;
}

static bool handle_set(uint8_t Flags)
{
    Z_UNUSED(Flags);

    /*
     * 8b    8b  8b     8b
     * flags set var_id value
     *       set x      32
     */
    uint8_t var_id;
    int8_t value;

    Z_READ_ARGU8(var_id, 0, 0);
    Z_READ_ARGI8(value, 1, 1);

    g_vars[var_id] = value;

    return true;
}

static bool handle_inc(uint8_t Flags)
{
    Z_UNUSED(Flags);

    /*
     * 8b    8b  8b     8b
     * flags inc var_id value
     *       inc x      16
     */
    uint8_t var_id;
    int8_t value;

    Z_READ_ARGU8(var_id, 0, 0);
    Z_READ_ARGI8(value, 1, 1);

    g_vars[var_id] = i8(g_vars[var_id] + value);

    return true;
}

void z_vm_setup(void)
{
    #define setOp(Index, Function, ArgBytes)          \
        g_ops[Index] = (ZOp){Function, 2 + ArgBytes};

    setOp(Z_OP_SPAWN, handle_spawn, 4);
    setOp(Z_OP_WAIT, handle_wait, 1);
    setOp(Z_OP_WAITCLEAR, handle_waitclear, 0);
    setOp(Z_OP_LOOP, handle_loop, 1);
    setOp(Z_OP_END, handle_end, 0);
    setOp(Z_OP_OVER, handle_over, 0);
    setOp(Z_OP_SET, handle_set, 2);
    setOp(Z_OP_INC, handle_inc, 2);

    z_vm_reset();
}

void z_vm_reset(void)
{
    g_pc = 0;
    g_wait = 0;

    for(uint8_t v = Z_VARS_NUM; v--; ) {
        g_vars[v] = 0;
    }
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
        g_pc = u16(g_pc + g_ops[op].bytes);
    }
}

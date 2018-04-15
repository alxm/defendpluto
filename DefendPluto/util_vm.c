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
#include "util_vm.h"

#include "data_levels.h"
#include "state.h"
#include "obj_enemy.h"
#include "obj_player.h"
#include "util_fps.h"
#include "util_pool.h"
#include "util_timer.h"

#define Z_DONE_LINGER_DS (8)

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
    Z_OP_CLEAR,
    Z_OP_SPAWN,
    Z_OP_DONE,
    Z_OP_NUM
} ZOpId;

typedef bool (ZOpCallback)(uint8_t Flags);

typedef struct {
    ZOpCallback* callback;
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
    int8_t vars[Z_LEVELS_VARS_NUM];
} g_vm;

static inline uint8_t vm_read(uint8_t Offset)
{
    return z_pgm_readU8(z_data_levels + g_vm.pc + Offset);
}

static inline uint8_t vm_readOp(void)
{
    return vm_read(0);
}

static inline uint8_t vm_readFlags(void)
{
    return vm_read(1);
}

static inline uint8_t vm_readArg(uint8_t ByteIndex)
{
    return vm_read(u8(2 + ByteIndex));
}

static int8_t vm_readArgI8(uint8_t Flags, uint8_t ArgIndex, uint8_t ByteIndex)
{
    int8_t value = i8(vm_readArg(ByteIndex));

    if(Flags & (1 << ArgIndex)) {
        value = g_vm.vars[value];
    }

    return value;
}

static uint8_t vm_readArgU8(uint8_t Flags, uint8_t ArgIndex, uint8_t ByteIndex)
{
    uint8_t value = vm_readArg(ByteIndex);

    if(Flags & (1 << ArgIndex)) {
        value = u8(g_vm.vars[value]);
    }

    return value;
}

static uint8_t vm_readArgU4H(uint8_t Flags, uint8_t ArgIndex, uint8_t ByteIndex)
{
    uint8_t value = vm_readArg(ByteIndex) >> 4;

    if(Flags & (1 << ArgIndex)) {
        value = u8(g_vm.vars[value]);
    }

    return value;
}

static uint8_t vm_readArgU4L(uint8_t Flags, uint8_t ArgIndex, uint8_t ByteIndex)
{
    uint8_t value = vm_readArg(ByteIndex) & 0xf;

    if(Flags & (1 << ArgIndex)) {
        value = u8(g_vm.vars[value]);
    }

    return value;
}

static bool op_over(uint8_t Flags)
{
    Z_UNUSED(Flags);

    /*
     * 8b
     * over
     * over
     */
    z_player.level = 0;
    z_state_setEx(Z_STATE_WIN, Z_SWIPE_HIDE, Z_SWIPE_SHOW);

    return false;
}

static bool op_set(uint8_t Flags)
{
    /*
     * 8b  8b    8b     8b
     * set flags var_id value
     * set       x      32
     */
    uint8_t var_id = vm_readArgU8(Flags, 0, 0);
    int8_t value = vm_readArgI8(Flags, 1, 1);

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
    uint8_t var_id = vm_readArgU8(Flags, 0, 0);
    int8_t value = vm_readArgI8(Flags, 1, 1);

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
    uint8_t var_id = vm_readArgU8(Flags, 0, 0);
    g_vm.vars[var_id] = !g_vm.vars[var_id];

    return true;
}

static bool op_loop(uint8_t Flags)
{
    /*
     * 8b   8b    8b
     * loop flags num_times
     * loop       10
     */
    uint8_t num_times = vm_readArgU8(Flags, 0, 0);

    if(num_times == 0) {
        for(uint8_t op = Z_OP_LOOP, loopCount = 1; loopCount > 0; ) {
            g_vm.pc = u16(g_vm.pc + g_ops[op].bytes);
            op = vm_readOp();

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
    uint8_t iteration = vm_readArgU8(Flags, 0, 0);

    if(g_vm.loopStack[g_vm.loopIndex].counter != iteration) {
        for(uint8_t op = Z_OP_ITER; op != Z_OP_ENDI; op = vm_readOp()) {
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

static bool nothingHappening(void)
{
    return z_pool_noActive(Z_POOL_BULLETE)
        && z_pool_noActive(Z_POOL_CIRCLE)
        && z_pool_noActive(Z_POOL_ENEMY)
        && z_pool_noActive(Z_POOL_PARTICLE);
}

static bool op_wait(uint8_t Flags)
{
    /*
     * 8b   8b    8b
     * wait flags ds
     * wait       25
     */
    if(nothingHappening()) {
        z_timer_stop(Z_TIMER_VM);
    } else if(!z_timer_running(Z_TIMER_VM)) {
        z_timer_start(Z_TIMER_VM, vm_readArgU8(Flags, 0, 0));
    } else if(z_timer_expired(Z_TIMER_VM)) {
        z_timer_stop(Z_TIMER_VM);
    }

    return !z_timer_running(Z_TIMER_VM);

}

static bool op_clear(uint8_t Flags)
{
    Z_UNUSED(Flags);

    /*
     * 8b
     * clear
     * clear
     */
    return nothingHappening();
}

static bool op_spawn(uint8_t Flags)
{
    /*
     * 8b    8b    8b      8b      4b      4b     4b
     * spawn flags x_coord y_coord type_id fly_id attack_id
     * spawn       40      -1      enemy0  down   target
     */
    int16_t x = vm_readArgI8(Flags, 0, 0);
    int16_t y = vm_readArgI8(Flags, 1, 1);
    uint8_t type_id = vm_readArgU4H(Flags, 2, 2);
    uint8_t fly_id = vm_readArgU4L(Flags, 3, 2);
    uint8_t attack_id = vm_readArgU4H(Flags, 4, 3);

    ZEnemy* e = z_pool_alloc(Z_POOL_ENEMY);

    if(e == NULL) {
        return false;
    }

    int16_t spriteWidth = z_sprite_getWidth(z_enemy_data[type_id].sprite);
    int16_t spriteHeight = z_sprite_getHeight(z_enemy_data[type_id].sprite);

    if(x < 0) {
        x = i16(x * spriteWidth + spriteWidth / 2);
    } else if(x > 100) {
        x = i16(Z_SCREEN_W + (x - 100) * spriteWidth - spriteWidth / 2);
    } else {
        x = i16((Z_SCREEN_W - 1) * x / 100);
    }

    if(y < 0) {
        y = i16(y * spriteHeight + spriteHeight / 2);
    } else if(y > 100) {
        y = i16(Z_SCREEN_H + (y - 100) * spriteHeight - spriteHeight / 2);
    } else {
        y = i16((Z_SCREEN_H - 1) * y / 100);
    }

    z_enemy_init(e, x, y, type_id, fly_id, attack_id);

    return true;
}

static bool op_done(uint8_t Flags)
{
    Z_UNUSED(Flags);

    /*
     * 8b
     * done
     * done
     */
    if(!z_timer_running(Z_TIMER_VM)) {
        if(!nothingHappening()) {
            return false;
        }

        z_timer_start(Z_TIMER_VM, Z_DONE_LINGER_DS);
    } else if(z_timer_expired(Z_TIMER_VM)) {
        z_timer_stop(Z_TIMER_VM);
        z_state_setEx(Z_STATE_NEXT, Z_SWIPE_HIDE, Z_SWIPE_SHOW);
    }

    return !z_timer_running(Z_TIMER_VM);
}

static void setOp(uint8_t Index, ZOpCallback* Function, uint8_t ArgBytes)
{
    g_ops[Index] = (ZOp){Function, u8(1 + (ArgBytes > 0 ? 1 + ArgBytes : 0))};
}

void z_vm_setup(void)
{
    setOp(Z_OP_OVER, op_over, 0);
    setOp(Z_OP_SET, op_set, 2);
    setOp(Z_OP_INC, op_inc, 2);
    setOp(Z_OP_FLIP, op_flip, 1);
    setOp(Z_OP_LOOP, op_loop, 1);
    setOp(Z_OP_END, op_end, 0);
    setOp(Z_OP_ITER, op_iter, 1);
    setOp(Z_OP_ENDI, op_endi, 0);
    setOp(Z_OP_WAIT, op_wait, 1);
    setOp(Z_OP_CLEAR, op_clear, 0);
    setOp(Z_OP_SPAWN, op_spawn, 4);
    setOp(Z_OP_DONE, op_done, 0);

    z_vm_reset();
}

void z_vm_reset(void)
{
    g_vm.pc = 0;
    g_vm.loopIndex = Z_LEVELS_NESTED_LOOPS_MAX;

    for(uint8_t v = Z_LEVELS_VARS_NUM; v--; ) {
        g_vm.vars[v] = 0;
    }
}

void z_vm_tick(void)
{
    while(true) {
        uint8_t op = vm_readOp();
        uint8_t flags = 0;

        if(g_ops[op].bytes > 1) {
            flags = vm_readFlags();
        }

        if(g_ops[op].callback(flags)) {
            op = vm_readOp();
            g_vm.pc = u16(g_vm.pc + g_ops[op].bytes);
        } else {
            break;
        }
    }
}

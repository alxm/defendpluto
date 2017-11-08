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

typedef struct {
    bool (*callback)(void);
    uint8_t bytes;
} ZInstruction;

static uint16_t g_pc = 0;
static uint8_t g_wait = 0;
static uint8_t g_counter = 0;
static bool g_freshOp = true;
static bool g_reset = false;

#define Z_READ(Offset) Z_PGM_READ_UINT8(z_data_levels[g_pc + Offset])

static bool handle_spawn(void)
{
    /*
     * 8b    8b      8b      4b        4b      4b      4b        8b
     * spawn x_coord y_coord sprite_id ai_id   ai_data num_units wait_between
     * spawn 64      -8      enemy0    nobrain 0       1         0
    */
    int8_t x = (int8_t)Z_READ(1);
    int8_t y = (int8_t)Z_READ(2);
    uint8_t sprite_id = Z_READ(3) >> 4;
    uint8_t ai_id = Z_READ(3) & 0xf;
    uint8_t ai_data = Z_READ(4) >> 4;
    uint8_t num_units = Z_READ(4) & 0xf;
    uint8_t wait_between = Z_READ(5);

    if(!g_freshOp) {
        num_units = g_counter;
    }

    if(num_units > 0) {
        ZEnemy* e = z_pool_alloc(Z_POOL_ENEMY);

        if(e == NULL) {
            return false;
        }

        z_enemy_init(e, x, y, sprite_id, ai_id, ai_data);

        num_units--;
    }

    if(num_units == 0) {
        return true;
    } else {
        g_wait = wait_between;
        g_counter = num_units;

        return false;
    }
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

static ZInstruction g_ops[] = {
    {handle_spawn, 6},
    {handle_wait, 2},
    {handle_waitclear, 1},
    {handle_over, 1},
};

void z_vm_tick(void)
{
    if(g_wait) {
        g_wait--;
        return;
    }

    if(g_reset) {
        g_pc = 0;
        g_freshOp = true;
        g_reset = false;
    }

    uint8_t instruction = Z_READ(0);

    if(g_ops[instruction].callback()) {
        g_pc = (uint16_t)(g_pc + g_ops[instruction].bytes);
        g_freshOp = true;
    } else {
        g_freshOp = false;
    }
}

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

#include "shared.h"
#include "util_pool.h"
#include "util_vm.h"
#include "obj_bullet.h"
#include "obj_enemy.h"
#include "obj_star.h"

typedef struct {
    bool (*callback)(void);
    uint8_t bytes;
} ZInstruction;

static unsigned g_pc = 0;
static unsigned g_wait = 0;

static const uint8_t g_data[] =
{
    0x00, 0x08, 0x08, 0x00, 0x01, 0x00,
    0x01, 0x1e,
    0x00, 0x20, 0x10, 0x00, 0x01, 0x00,
    0x01, 0x1e,
    0x00, 0x40, 0x04, 0x00, 0x01, 0x00,
    0x01, 0x1e,
    0x00, 0x08, 0x18, 0x00, 0x01, 0x00,
    0x01, 0x1e,
    0x00, 0x20, 0x14, 0x00, 0x01, 0x00,
    0x01, 0x1e,
    0x00, 0x40, 0x14, 0x00, 0x01, 0x00,
    0x01, 0x1e,
    0xff,
}
;

static bool handle_spawn(void)
{
    /*
     * 8b    8b      8b      4b          4b      4b      4b        8b
     * spawn x_coord y_coord object_type ai_type ai_data num_units wait_between
     * spawn 64      0       enemy1      nobrain 0       1         0
    */
    int8_t x = (int8_t)g_data[g_pc + 1];
    int8_t y = (int8_t)g_data[g_pc + 2];
    uint8_t object_type = g_data[g_pc + 3] >> 4;
    uint8_t ai_type = g_data[g_pc + 3] & 0xf;
    uint8_t ai_data = g_data[g_pc + 4] >> 4;
    uint8_t num_units = g_data[g_pc + 4] & 0xf;
    uint8_t wait_between = g_data[g_pc + 5];

    A_UNUSED(ai_type);
    A_UNUSED(ai_data);
    A_UNUSED(num_units);
    A_UNUSED(wait_between);

    switch(object_type) {
        case 0: {
            ZEnemy* e = z_pool_alloc(z_pool[Z_POOL_ENEMY]);

            if(e == NULL) {
                return false;
            }

            z_enemy_init(e, x, y);
        } break;
    }

    return true;
}

static bool handle_wait(void)
{
    /*
     * 8b   8b
     * wait frames
     * wait 30
    */
    g_wait = g_data[g_pc + 1];

    return true;
}

static ZInstruction g_ops[] = {
    {handle_spawn, 6},
    {handle_wait, 2},
};

void z_vm_tick(void)
{
    if(g_wait) {
        g_wait--;
        return;
    }

    if(g_data[g_pc] == 0xff) {
        return;
    }

    uint8_t instruction = g_data[g_pc];

    if(g_ops[instruction].callback()) {
        g_pc += g_ops[instruction].bytes;
    }
}

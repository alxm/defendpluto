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
#include "obj_bullet.h"
#include "obj_enemy.h"
#include "obj_star.h"

#define SHOOT_EVERY_N_FRAMES (S_FPS / 4)

static struct {
    SButton up, down, left, right, a, b;
    int8_t x, y;
    unsigned lastShot;
} g_context;

static unsigned g_pc = 0;
static const uint8_t g_data[] =
{
    0x00, 0x08, 0x08, 0x00, 0x01, 0x00,
    0x00, 0x20, 0x10, 0x00, 0x01, 0x00,
    0x00, 0x40, 0x04, 0x00, 0x01, 0x00,
    0xff,
}
;

void handle_spawn(void)
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

            z_enemy_init(e, x, y);
        } break;
    }
}

typedef struct {
    void (*callback)(void);
    uint8_t bytes;
} ZInstruction;

ZInstruction g_ops[] = {
    {handle_spawn, 6},
};

void eval(void)
{
    while(g_data[g_pc] != 0xff) {
        uint8_t instruction = g_data[g_pc];

        g_ops[instruction].callback();
        g_pc += g_ops[instruction].bytes;
    }
}

void loop_setup(void)
{
    s_setup();
    z_pool_setup();

    g_context.up = s_buttons[S_BUTTON_UP];
    g_context.down = s_buttons[S_BUTTON_DOWN];
    g_context.left = s_buttons[S_BUTTON_LEFT];
    g_context.right = s_buttons[S_BUTTON_RIGHT];
    g_context.a = s_buttons[S_BUTTON_A];
    g_context.b = s_buttons[S_BUTTON_B];

    g_context.x = S_WIDTH / 2;
    g_context.y = S_HEIGHT / 2;

    g_context.lastShot = s_fps_getCounter();

    eval();
}

void loop_tick(void)
{
    if(s_button_pressed(g_context.up) && g_context.y > 0) {
        g_context.y--;
    } else if(s_button_pressed(g_context.down) && g_context.y < S_HEIGHT - 1) {
        g_context.y++;
    }

    if(s_button_pressed(g_context.left) && g_context.x > 0) {
        g_context.x--;
    } else if(s_button_pressed(g_context.right) && g_context.x < S_WIDTH - 1) {
        g_context.x++;
    }

    if(s_button_pressed(g_context.a)) {
        if(s_fps_getCounter() - g_context.lastShot >= SHOOT_EVERY_N_FRAMES) {
            ZBullet* b = z_pool_alloc(z_pool[Z_POOL_BULLET]);

            if(b) {
                z_bullet_init(b, g_context.x, g_context.y, -2);
            }

            g_context.lastShot = s_fps_getCounter();
        }
    } else {
        g_context.lastShot = s_fps_getCounter() - SHOOT_EVERY_N_FRAMES;
    }

    if(s_fps_isNthFrame(S_FPS / 5)) {
        z_pool_tick(z_pool[Z_POOL_STAR], z_star_tick);

        if(rand() % (S_HEIGHT / Z_STARS_NUM / Z_STAR_AVG_SPEED) == 0) {
            ZStar* star = z_pool_alloc(z_pool[Z_POOL_STAR]);

            if(star != NULL) {
                z_star_init(star);
            }
        }
    }

    z_pool_tick(z_pool[Z_POOL_BULLET], z_bullet_tick);
    z_pool_tick(z_pool[Z_POOL_ENEMY], z_enemy_tick);
}

void loop_draw(void)
{
    s_draw_fill(false);
    z_pool_draw(z_pool[Z_POOL_STAR], z_star_draw);
    z_pool_draw(z_pool[Z_POOL_BULLET], z_bullet_draw);
    z_pool_draw(z_pool[Z_POOL_ENEMY], z_enemy_draw);
    s_draw_rectangle(g_context.x - 3, g_context.y - 4, 6, 8, true);
}

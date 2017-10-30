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
#include "util_fix.h"
#include "util_pool.h"
#include "util_vm.h"
#include "obj_bullet.h"
#include "obj_enemy.h"
#include "obj_star.h"

#define SHOOT_EVERY_N_FRAMES (S_FPS / 4)

static struct {
    SButton up, down, left, right, a, b;
    ZFix x, y;
    unsigned lastShot;
} g_context;

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

    g_context.x = z_fix_itofix(S_WIDTH / 2);
    g_context.y = z_fix_itofix(S_HEIGHT / 2);

    g_context.lastShot = s_fps_getCounter();
}

void loop_tick(void)
{
    z_vm_tick();

    if(s_button_pressed(g_context.up) && g_context.y > 0) {
        g_context.y = (ZFix)(g_context.y - Z_FIX_ONE);
    } else if(s_button_pressed(g_context.down) && z_fix_fixtoi(g_context.y) < S_HEIGHT - 1) {
        g_context.y = (ZFix)(g_context.y + Z_FIX_ONE);
    }

    if(s_button_pressed(g_context.left) && g_context.x > 0) {
        g_context.x = (ZFix)(g_context.x - Z_FIX_ONE);
    } else if(s_button_pressed(g_context.right) && z_fix_fixtoi(g_context.x) < S_WIDTH - 1) {
        g_context.x = (ZFix)(g_context.x + Z_FIX_ONE);
    }

    if(s_button_pressed(g_context.a)) {
        if(s_fps_getCounter() - g_context.lastShot >= SHOOT_EVERY_N_FRAMES) {
            ZBullet* b = z_pool_alloc(z_pool[Z_POOL_BULLET]);

            if(b) {
                z_bullet_init(b, g_context.x, g_context.y, z_fix_itofix(-2));
            }

            g_context.lastShot = s_fps_getCounter();
        }
    } else {
        g_context.lastShot = s_fps_getCounter() - SHOOT_EVERY_N_FRAMES;
    }

    z_pool_tick(z_pool[Z_POOL_STAR], z_star_tick);
    z_pool_tick(z_pool[Z_POOL_BULLET], z_bullet_tick);
    z_pool_tick(z_pool[Z_POOL_ENEMY], z_enemy_tick);

    if(rand() % (2 * S_HEIGHT / Z_STARS_NUM) == 0) {
        ZStar* star = z_pool_alloc(z_pool[Z_POOL_STAR]);

        if(star != NULL) {
            z_star_init(star);
        }
    }
}

void loop_draw(void)
{
    s_draw_fill(false);
    z_pool_draw(z_pool[Z_POOL_STAR], z_star_draw);
    z_pool_draw(z_pool[Z_POOL_BULLET], z_bullet_draw);
    z_pool_draw(z_pool[Z_POOL_ENEMY], z_enemy_draw);

    int8_t x = z_fix_fixtoi(g_context.x);
    int8_t y = z_fix_fixtoi(g_context.y);

    s_draw_rectangle(x - 3, y - 4, 6, 8, true);
}

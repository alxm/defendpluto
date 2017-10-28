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

static struct {
    int8_t x, y;
    SButton up, down, left, right, a, b;
    ZPool* starPool;
    ZPool* bulletPool;
    ZPool* enemyPool;
} g_context;

void loop_setup(void)
{
    s_setup();
    z_pool_setup();

    g_context.x = S_WIDTH / 2;
    g_context.y = S_HEIGHT / 2;

    g_context.up = s_buttons[S_BUTTON_UP];
    g_context.down = s_buttons[S_BUTTON_DOWN];
    g_context.left = s_buttons[S_BUTTON_LEFT];
    g_context.right = s_buttons[S_BUTTON_RIGHT];
    g_context.a = s_buttons[S_BUTTON_A];
    g_context.b = s_buttons[S_BUTTON_B];

    g_context.starPool = z_pool_get(Z_POOL_STAR);
    g_context.bulletPool = z_pool_get(Z_POOL_BULLET);
    g_context.enemyPool = z_pool_get(Z_POOL_ENEMY);

    for(int i = 0; i < Z_ENEMIES_NUM; i++) {
        ZEnemy* e = z_pool_alloc(g_context.enemyPool);

        z_enemy_init(e,
                     (int8_t)(rand() % S_WIDTH),
                     (int8_t)(rand() % S_HEIGHT));
    }
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

    if(s_button_pressed(g_context.a) && s_fps_isNthFrame(S_FPS / 4)) {
        ZBullet* b = z_pool_alloc(g_context.bulletPool);

        if(b) {
            z_bullet_init(b, g_context.x, g_context.y, -2);
        }
    }

    if(s_fps_isNthFrame(S_FPS / 5)) {
        z_pool_tick(g_context.starPool, z_star_tick);

        if(rand() % (S_HEIGHT / Z_STARS_NUM / Z_STAR_AVG_SPEED) == 0) {
            ZStar* star = z_pool_alloc(g_context.starPool);

            if(star != NULL) {
                z_star_init(star);
            }
        }
    }

    z_pool_tick(g_context.bulletPool, z_bullet_tick);
    z_pool_tick(g_context.enemyPool, z_enemy_tick);
}

void loop_draw(void)
{
    s_draw_fill(false);
    z_pool_draw(g_context.starPool, z_star_draw);
    z_pool_draw(g_context.bulletPool, z_bullet_draw);
    z_pool_draw(g_context.enemyPool, z_enemy_draw);
    s_draw_rectangle(g_context.x - 3, g_context.y - 4, 6, 8, true);
}

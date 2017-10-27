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
#include "obj_bullet.h"
#include "obj_star.h"

static struct {
    int16_t x, y;
    SButton up, down, left, right, a, b;
    Z_POOL_DECLARE(ZStar, Z_STARS_NUM, stars) starPool;
    Z_POOL_DECLARE(ZBullet, Z_BULLETS_NUM, bullets) bulletPool;
} g_context;

static void generic_tick(ZPool* Pool, bool (*Callback)(ZPoolObject*))
{
    ZPoolObject* last = NULL;

    for(ZPoolObject* o = Pool->activeList; o != NULL; ) {
        if(Callback(o)) {
            o = z_pool_release(Pool, o, last);
        } else {
            last = o;
            o = o->next;
        }
    }
}

static void generic_draw(ZPool* Pool, void (*Callback)(ZPoolObject*))
{
    for(ZPoolObject* o = Pool->activeList; o != NULL; o = o->next) {
        Callback(o);
    }
}

void loop_setup(void)
{
    g_context.x = S_WIDTH / 2;
    g_context.y = S_HEIGHT / 2;

    g_context.up = s_buttons[S_BUTTON_UP];
    g_context.down = s_buttons[S_BUTTON_DOWN];
    g_context.left = s_buttons[S_BUTTON_LEFT];
    g_context.right = s_buttons[S_BUTTON_RIGHT];
    g_context.a = s_buttons[S_BUTTON_A];
    g_context.b = s_buttons[S_BUTTON_B];

    z_pool_init(&g_context.starPool.generic, sizeof(ZStar), Z_STARS_NUM);
    z_pool_init(&g_context.bulletPool.generic, sizeof(ZBullet), Z_BULLETS_NUM);
}

void loop_tick(void)
{
    if(s_button_pressed(g_context.up)) {
        g_context.y--;
    } else if(s_button_pressed(g_context.down)) {
        g_context.y++;
    }

    if(s_button_pressed(g_context.left)) {
        g_context.x--;
    } else if(s_button_pressed(g_context.right)) {
        g_context.x++;
    }

    if(s_button_pressed(g_context.a) && s_fps_isNthFrame(6)) {
        ZBullet* b = z_pool_alloc(&g_context.bulletPool.generic);

        if(b) {
            z_bullet_init(b,
                          g_context.x,
                          (int16_t)(g_context.y << FIX_PRECISION_BITS),
                          -1);
        }
    }

    generic_tick(&g_context.starPool.generic, z_star_tick);
    generic_tick(&g_context.bulletPool.generic, z_bullet_tick);

    if(rand() % (S_HEIGHT * Z_STAR_SPEED_DIV / Z_STARS_NUM) == 0) {
        ZStar* star = z_pool_alloc(&g_context.starPool.generic);

        if(star != NULL) {
            z_star_init(star);
        }
    }
}

void loop_draw(void)
{
    s_draw_fill(false);
    generic_draw(&g_context.starPool.generic, z_star_draw);
    generic_draw(&g_context.bulletPool.generic, z_bullet_draw);
    s_draw_rectangle(g_context.x - 3, g_context.y - 4, 6, 8, true);
}

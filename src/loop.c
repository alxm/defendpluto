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
#include "obj_star.h"

static struct {
    int x, y;
    SButton up, down, left, right;
    Z_POOL_DECLARE(ZStar, NUM_STARS, stars) starPool;
} g_context;

static void loop_stars_tick(void);
static void loop_stars_draw(void);

void loop_setup(void)
{
    g_context.x = S_WIDTH / 2;
    g_context.y = S_HEIGHT / 2;

    g_context.up = s_buttons[S_BUTTON_UP];
    g_context.down = s_buttons[S_BUTTON_DOWN];
    g_context.left = s_buttons[S_BUTTON_LEFT];
    g_context.right = s_buttons[S_BUTTON_RIGHT];

    z_pool_init(&g_context.starPool.generic, sizeof(ZStar), NUM_STARS);
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

    loop_stars_tick();
}

void loop_draw(void)
{
    s_draw_fill(false);
    loop_stars_draw();
    s_draw_rectangle(g_context.x - 3, g_context.y - 4, 6, 8, true);
}

static void loop_stars_tick(void)
{
    ZStar* last = NULL;

    for(ZStar* star = g_context.starPool.stars.activeList; star != NULL; ) {
        if(z_star_tick(star)) {
            star = z_pool_release(&g_context.starPool.generic, star, last);
        } else {
            last = star;
            star = star->poolObject.next;
        }
    }

    if(rand() % (S_HEIGHT * STAR_SPEED_DIV / NUM_STARS) == 0) {
        ZStar* star = z_pool_alloc(&g_context.starPool.generic);

        if(star != NULL) {
            z_star_init(star);
        }
    }
}

static void loop_stars_draw(void)
{
    for(ZStar* star = g_context.starPool.stars.activeList;
        star != NULL;
        star = star->poolObject.next) {

        z_star_draw(star);
    }
}

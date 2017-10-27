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

#define NUM_STARS 24
#define STAR_SPEED_DIV 4

typedef int16_t fix;
#define FIX_PRECISION_BITS 8
#define FIX_ONE (fix)(1 << FIX_PRECISION_BITS)

typedef struct {
    ZPoolObject poolObject;
    fix x, y;
    fix speed;
} ZStar;

static struct {
    int x, y;
    SButton up, down, left, right;
    Z_POOL_DECLARE(ZStar, NUM_STARS, stars);
} g_context;

static void z_star_new(void)
{
    ZStar* star = z_pool_alloc(&g_context.stars);

    if(star == NULL) {
        return;
    }

    star->x = (fix)(rand() % S_WIDTH);
    star->y = 0;
    star->speed = (fix)(FIX_ONE / STAR_SPEED_DIV / 2
                            + (rand() % (FIX_ONE / STAR_SPEED_DIV)));
}

static void z_star_tick(void)
{
    ZStar* star = g_context.stars.activeList;
    ZStar* last = NULL;

    while(star != NULL) {
        star->y = (fix)(star->y + star->speed);

        if(star->y >> FIX_PRECISION_BITS >= S_HEIGHT) {
            star = z_pool_release(&g_context.stars, star, last);
        } else {
            last = star;
            star = star->poolObject.next;
        }
    }

    if(rand() % (S_HEIGHT * STAR_SPEED_DIV / NUM_STARS) == 0) {
        z_star_new();
    }
}

static void z_star_draw(void)
{
    for(ZStar* s = g_context.stars.activeList;
        s != NULL;
        s = s->poolObject.next) {

        s_draw_pixel(s->x, s->y >> FIX_PRECISION_BITS, true);
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

    z_pool_init(&g_context.stars, sizeof(ZStar), NUM_STARS);
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

    z_star_tick();
}

void loop_draw(void)
{
    s_draw_fill(false);
    s_draw_rectangle(g_context.x - 3, g_context.y - 4, 6, 8, true);
    z_star_draw();
}

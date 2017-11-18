/*
    Copyright 2017 Alex Margarit <alex@alxm.org>

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
#include "loop.h"
#include "loop_game.h"
#include "loop_title.h"
#include "util_enemy.h"
#include "util_font.h"
#include "util_graphics.h"
#include "util_pool.h"
#include "util_vm.h"

typedef struct {
    ZStateCallback* init;
    ZStateCallback* free;
    ZStateCallback* tick;
    ZStateCallback* draw;
} ZState;

static ZState g_states[Z_STATE_NUM] = {
    {
        z_loop_title_init,
        z_loop_title_free,
        z_loop_title_tick,
        z_loop_title_draw
    },
    {
        z_loop_game_init,
        z_loop_game_free,
        z_loop_game_tick,
        z_loop_game_draw
    },
};

static uint8_t g_state = Z_STATE_NUM;

void z_loop_setup(void)
{
    z_platform_setup();
    z_enemy_setup();
    z_font_setup();
    z_graphics_setup();
    z_pool_setup();
    z_vm_setup();

    z_loop_setState(Z_STATE_GAME);
}

void z_loop_tick(void)
{
    z_platform_tick();
    g_states[g_state].tick();
}

void z_loop_draw(void)
{
    g_states[g_state].draw();
    z_platform_draw();
}

void z_loop_setState(uint8_t State)
{
    if(g_state < Z_STATE_NUM) {
        g_states[g_state].free();
    }

    g_state = State;
    g_states[g_state].init();
}

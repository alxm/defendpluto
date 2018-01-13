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
#include "loop.h"
#include "loop_died.h"
#include "loop_doors.h"
#include "loop_intro.h"
#include "loop_new.h"
#include "loop_next.h"
#include "loop_over.h"
#include "loop_play.h"
#include "loop_title.h"
#include "util_fix.h"
#include "util_font.h"
#include "util_graphics.h"
#include "util_input.h"
#include "util_pool.h"
#include "util_screen.h"
#include "util_str.h"
#include "util_timer.h"
#include "util_vm.h"
#include "obj_enemy.h"
#include "obj_player.h"
#include "obj_star.h"

typedef struct {
    ZStateCallback* init;
    ZStateCallback* tick;
    ZStateCallback* draw;
} ZState;

static ZState g_states[Z_STATE_NUM] = {
    {
        z_loop_died_init,
        z_loop_died_tick,
        z_loop_died_draw
    },
    {
        z_loop_doors_close_init,
        z_loop_doors_close_tick,
        z_loop_doors_draw
    },
    {
        z_loop_doors_open_init,
        z_loop_doors_intro_tick,
        z_loop_doors_intro_draw
    },
    {
        z_loop_doors_open_init,
        z_loop_doors_open_tick,
        z_loop_doors_draw
    },
    {
        z_loop_intro_init,
        z_loop_intro_tick,
        z_loop_intro_draw
    },
    {
        z_loop_new_init,
        NULL,
        NULL
    },
    {
        z_loop_next_init,
        z_loop_next_tick,
        z_loop_next_draw
    },
    {
        NULL,
        z_loop_over_tick,
        z_loop_over_draw
    },
    {
        NULL,
        z_loop_play_tick,
        z_loop_play_draw
    },
    {
        z_loop_title_init,
        z_loop_title_tick,
        z_loop_title_draw
    },
};

static int8_t g_state, g_lastState;

void z_loop_setup(void)
{
    g_state = Z_STATE_INVALID;
    g_lastState = Z_STATE_INVALID;

    z_platform_setup();
    z_input_reset();
    z_screen_reset();
    z_font_setup();
    z_graphics_setup();
    z_str_setup();
    z_pool_setup();
    z_vm_setup();
    z_enemy_setup();
    z_star_setup();
    z_player_resetPosition();

    z_loop_setState(Z_STATE_INTRO);
}

void z_loop_tick(void)
{
    z_platform_tick();
    z_timer_tick();

    if(g_states[g_state].tick) {
        g_states[g_state].tick();
    }
}

void z_loop_draw(void)
{
    if(g_states[g_state].draw) {
        g_states[g_state].draw();
    }

    z_platform_draw();
}

int8_t z_loop_getLastState(void)
{
    return g_lastState;
}

void z_loop_setState(int8_t State)
{
    g_lastState = g_state;
    g_state = State;

    if(g_states[g_state].init) {
        g_states[g_state].init();
    }
}

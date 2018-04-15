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
#include "state.h"

#include "state_died.h"
#include "state_intro.h"
#include "state_new.h"
#include "state_next.h"
#include "state_over.h"
#include "state_pause.h"
#include "state_play.h"
#include "state_title.h"
#include "state_win.h"
#include "obj_enemy.h"
#include "obj_player.h"
#include "obj_star.h"
#include "util_font.h"
#include "util_input.h"
#include "util_pool.h"
#include "util_screen.h"
#include "util_str.h"
#include "util_timer.h"
#include "util_vm.h"

typedef struct {
    ZStateInit* init;
    ZStateTick* tick;
    ZStateDraw* draw;
} ZState;

static ZState g_states[Z_STATE_NUM] = {
    [Z_STATE_DIED] = {
        z_state_died_init,
        z_state_died_tick,
        z_state_died_draw
    },
    [Z_STATE_INTRO] = {
        z_state_intro_init,
        z_state_intro_tick,
        z_state_intro_draw
    },
    [Z_STATE_NEW] = {
        z_state_new_init,
        NULL,
        NULL
    },
    [Z_STATE_NEXT] = {
        z_state_next_init,
        z_state_next_tick,
        z_state_next_draw
    },
    [Z_STATE_OVER] = {
        NULL,
        z_state_over_tick,
        z_state_over_draw
    },
    [Z_STATE_PAUSE] = {
        NULL,
        z_state_pause_tick,
        z_state_pause_draw
    },
    [Z_STATE_PLAY] = {
        NULL,
        z_state_play_tick,
        z_state_play_draw
    },
    [Z_STATE_TITLE] = {
        z_state_title_init,
        z_state_title_tick,
        z_state_title_draw
    },
    [Z_STATE_WIN] = {
        z_state_win_init,
        z_state_win_tick,
        z_state_win_draw
    },
};

static struct {
    ZStateId current;
    ZStateId next;
} g_state;

static struct {
    ZSwipeId swipeOut;
    ZSwipeId swipeIn;
    uint8_t height;
} g_swipe;

typedef void (ZSwipeInit)(void);
typedef bool (ZSwipeTick)(void);
typedef void (ZSwipeDraw)(void);

#define Z_SLIDE_CLOSE_INC (2)
#define Z_SLIDE_OPEN_INC  (2)

static void swipeHideInit(void)
{
    g_swipe.height = 0;
}

static bool swipeHideTick(void)
{
    g_swipe.height = u8(g_swipe.height + Z_SLIDE_CLOSE_INC);

    return g_swipe.height > Z_SCREEN_H / 2;
}

static void swipeShowInit(void)
{
    g_swipe.height = Z_SCREEN_H / 2;
}

static bool swipeShowTick(void)
{
    g_swipe.height = u8(g_swipe.height - Z_SLIDE_OPEN_INC);

    return g_swipe.height == 0;
}

static void swipeDraw(void)
{
    z_draw_rectangle(0, 0, Z_SCREEN_W, g_swipe.height, Z_COLOR_BLUE);
    z_draw_hline(0, Z_SCREEN_W - 1, i16(g_swipe.height - 1), Z_COLOR_YELLOW);

    z_draw_hline(0,
                 Z_SCREEN_W - 1,
                 i16(Z_SCREEN_H - g_swipe.height),
                 Z_COLOR_YELLOW);
    z_draw_rectangle(0,
                     i16(Z_SCREEN_H - g_swipe.height + 1),
                     Z_SCREEN_W,
                     g_swipe.height,
                     Z_COLOR_BLUE);
}

static struct {
    ZSwipeInit* init;
    ZSwipeTick* tick;
    ZSwipeDraw* draw;
} g_swipeCallbacks[Z_SWIPE_NUM] = {
    [Z_SWIPE_HIDE] = {swipeHideInit, swipeHideTick, swipeDraw},
    [Z_SWIPE_SHOW] = {swipeShowInit, swipeShowTick, swipeDraw},
};

void z_state_setup(void)
{
    g_state.current = Z_STATE_INVALID;
    g_state.next = Z_STATE_INVALID;

    g_swipe.swipeOut = Z_SWIPE_INVALID;
    g_swipe.swipeIn = Z_SWIPE_INVALID;

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

    z_state_setState(Z_STATE_INTRO);
}

void z_state_tick(void)
{
    if(g_state.next != Z_STATE_INVALID) {
        if(g_swipe.swipeOut != Z_SWIPE_INVALID) {
            if(g_swipeCallbacks[g_swipe.swipeOut].tick()) {
                g_swipe.swipeOut = Z_SWIPE_INVALID;
            }
        }

        if(g_swipe.swipeOut == Z_SWIPE_INVALID) {
            g_state.current = g_state.next;
            g_state.next = Z_STATE_INVALID;

            if(g_states[g_state.current].init) {
                g_states[g_state.current].init();
            }

            if(g_swipe.swipeIn != Z_SWIPE_INVALID) {
                g_swipeCallbacks[g_swipe.swipeIn].init();
            }
        }
    }

    if(g_state.next == Z_STATE_INVALID && g_swipe.swipeIn != Z_SWIPE_INVALID) {
        if(g_swipeCallbacks[g_swipe.swipeIn].tick()) {
            g_swipe.swipeIn = Z_SWIPE_INVALID;
        }
    }

    z_screen_tick();
    z_timer_tick();

    if(g_states[g_state.current].tick) {
        g_states[g_state.current].tick(g_state.next == Z_STATE_INVALID);
    }
}

void z_state_draw(void)
{
    if(g_states[g_state.current].draw) {
        g_states[g_state.current].draw();
    }

    if(g_swipe.swipeOut != Z_SWIPE_INVALID) {
        g_swipeCallbacks[g_swipe.swipeOut].draw();
    } else if(g_swipe.swipeIn != Z_SWIPE_INVALID
        && g_state.next == Z_STATE_INVALID) {

        g_swipeCallbacks[g_swipe.swipeIn].draw();
    }
}

void z_state_setState(ZStateId NewState)
{
    g_state.next = NewState;
}

void z_state_setStateEx(ZStateId NewState, ZSwipeId SwipeOut, ZSwipeId SwipeIn)
{
    z_state_setState(NewState);

    g_swipe.swipeOut = SwipeOut;
    g_swipe.swipeIn = SwipeIn;

    if(SwipeOut != Z_SWIPE_INVALID) {
        g_swipeCallbacks[SwipeOut].init();
    }
}

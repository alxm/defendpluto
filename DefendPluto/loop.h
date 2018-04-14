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

#pragma once

Z_EXTERN_C_START

typedef enum Z_ENUM_PACK {
    Z_STATE_INVALID = -1,
    Z_STATE_DIED,
    Z_STATE_INTRO,
    Z_STATE_NEW,
    Z_STATE_NEXT,
    Z_STATE_OVER,
    Z_STATE_PAUSE,
    Z_STATE_PLAY,
    Z_STATE_TITLE,
    Z_STATE_WIN,
    Z_STATE_NUM
} ZStateId;

typedef enum Z_ENUM_PACK {
    Z_SWIPE_INVALID = -1,
    Z_SWIPE_HIDE,
    Z_SWIPE_SHOW,
    Z_SWIPE_NUM
} ZSwipeId;

typedef void (ZStateInit)(void);
typedef void (ZStateTick)(bool Active);
typedef void (ZStateDraw)(void);

extern void z_loop_setup(void);

extern void z_loop_tick(void);
extern void z_loop_draw(void);

extern void z_loop_setState(ZStateId NewState);
extern void z_loop_setStateEx(ZSwipeId SwipeOut, ZStateId NewState, ZSwipeId SwipeIn);

Z_EXTERN_C_END

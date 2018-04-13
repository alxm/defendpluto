/*
    Copyright 2018 Alex Margarit <alex@alxm.org>

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

#include "loop.h"

extern ZStateInit z_loop_swipe_hide_init;
extern ZStateInit z_loop_swipe_show_init;
extern ZStateTick z_loop_swipe_hide_tick;
extern ZStateTick z_loop_swipe_intro_tick;
extern ZStateTick z_loop_swipe_show_tick;
extern ZStateDraw z_loop_swipe_draw;
extern ZStateDraw z_loop_swipe_intro_draw;

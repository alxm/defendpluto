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

typedef enum Z_ENUM_PACK {
    Z_SWIPE_INVALID = -1,
    Z_SWIPE_HIDE,
    Z_SWIPE_SHOW,
    Z_SWIPE_NUM
} ZSwipeId;

extern void z_swipe_init(ZSwipeId Swipe);
extern bool z_swipe_tick(ZSwipeId Swipe);
extern void z_swipe_draw(ZSwipeId Swipe);

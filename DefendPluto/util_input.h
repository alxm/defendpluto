/*
    Copyright 2017-2018 Alex Margarit <alex@alxm.org>
    This file is part of Defend Pluto, a video game.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

Z_EXTERN_C_START

typedef enum Z_PACK_ENUM {
    Z_BUTTON_INVALID = -1,
    Z_BUTTON_UP,
    Z_BUTTON_DOWN,
    Z_BUTTON_LEFT,
    Z_BUTTON_RIGHT,
    Z_BUTTON_A,
    Z_BUTTON_B,
    Z_BUTTON_MENU,
    Z_BUTTON_NUM
} ZButtonId;

extern void z_input_reset(void);

extern bool z_button_pressed(ZButtonId Button);
extern void z_button_release(ZButtonId Button);
extern bool z_button_pressedOnce(ZButtonId Button);

Z_EXTERN_C_END

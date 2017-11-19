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

Z_EXTERN_C_START

typedef enum {
    Z_BUTTON_INVALID = -1,
    Z_BUTTON_UP,
    Z_BUTTON_DOWN,
    Z_BUTTON_LEFT,
    Z_BUTTON_RIGHT,
    Z_BUTTON_A,
    Z_BUTTON_B,
    Z_BUTTON_NUM
} ZButtonId;

extern bool z_button_pressed(uint8_t Button);
extern void z_button_release(uint8_t Button);

Z_EXTERN_C_END

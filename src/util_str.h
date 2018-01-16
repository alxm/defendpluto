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

typedef enum {
    Z_STR_INVALID = -1,
    Z_STR_PRESS_A,
    Z_STR_END,
    Z_STR_WIN,
    Z_STR_THANKS,
    Z_STR_LEVEL_CLEARED_1,
    Z_STR_LEVEL_CLEARED_2,
    Z_STR_NUM
} ZStringId;

typedef struct {
    const char* text;
    uint16_t len;
} ZString;

extern ZString z_strings[Z_STR_NUM];

extern void z_str_setup(void);

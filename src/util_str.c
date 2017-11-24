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
#include "util_str.h"

const char* z_strings[Z_STR_NUM];

#define SET_STR(Index, Text)                \
{                                           \
    PROGMEM static const char str[] = Text; \
    z_strings[Index] = str;                 \
}

void z_str_setup(void)
{
    SET_STR(Z_STR_PRESS_FIRE1, "Press FIRE");
    SET_STR(Z_STR_PRESS_FIRE2, "Press FIRE to go on");
    SET_STR(Z_STR_END1, "Pluto fell to the");
    SET_STR(Z_STR_END2, "Proxima Centauri");
    SET_STR(Z_STR_END3, "invaders");
}

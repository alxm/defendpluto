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
#include "util_str.h"

ZString z_strings[Z_STR_NUM];

#define SET_STR(Index, Text)                 \
{                                            \
    PROGMEM static const char str[] = Text;  \
                                             \
    z_strings[Index].text = str;             \
    z_strings[Index].len = sizeof(Text) - 1; \
}

void z_str_setup(void)
{
    SET_STR(Z_STR_PRESS_A, "Press A");
    SET_STR(Z_STR_END, "Pluto fell to the Proxima Centauri invaders");
}

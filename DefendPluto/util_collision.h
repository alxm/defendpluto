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

static inline bool z_collision_boxAndBox(int16_t X1, int16_t Y1, int8_t W1, int8_t H1, int16_t X2, int16_t Y2, int8_t W2, int8_t H2)
{
    X1 = i16(X1 - W1 / 2);
    Y1 = i16(Y1 - H1 / 2);

    X2 = i16(X2 - W2 / 2);
    Y2 = i16(Y2 - H2 / 2);

    return !(Y1 >= Y2 + H2 || Y2 >= Y1 + H1 || X1 >= X2 + W2 || X2 >= X1 + W1);
}

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

extern bool z_collision_checkEnemyShips(int16_t X, int16_t Y, int8_t W, int8_t H, uint8_t Damage, bool AllowMultipleCollisions);
extern bool z_collision_checkPlayer(int16_t X, int16_t Y, int8_t W, int8_t H, uint8_t Damage);

static inline bool z_collision_pointInBox(int16_t X, int16_t Y, int16_t BoxX, int16_t BoxY, int8_t BoxW, int8_t BoxH)
{
    BoxX = i16(BoxX - BoxW / 2);
    BoxY = i16(BoxY - BoxH / 2);

    return X >= BoxX && X < BoxX + BoxW && Y >= BoxY && Y < BoxY + BoxH;
}

static inline bool z_collision_boxAndBox(int16_t X1, int16_t Y1, int8_t W1, int8_t H1, int16_t X2, int16_t Y2, int8_t W2, int8_t H2)
{
    X1 = i16(X1 - W1 / 2);
    Y1 = i16(Y1 - H1 / 2);

    X2 = i16(X2 - W2 / 2);
    Y2 = i16(Y2 - H2 / 2);

    return !(Y1 >= Y2 + H2 || Y2 >= Y1 + H1 || X1 >= X2 + W2 || X2 >= X1 + W1);
}

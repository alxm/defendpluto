/*
    Copyright 2017 Alex Margarit <alex@alxm.org>

    arduboy-shooter is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    arduboy-shooter is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with arduboy-shooter.  If not, see <http://www.gnu.org/licenses/>.
*/

static inline int8_t z_random_int8(int8_t Max)
{
    return i8(rand() % Max);
}

static inline uint8_t z_random_uint8(uint8_t Max)
{
    return u8(rand() % Max);
}

static inline int16_t z_random_int16(int16_t Max)
{
    return i16(rand() % Max);
}

static inline uint16_t z_random_uint16(uint16_t Max)
{
    return u16(rand() % Max);
}

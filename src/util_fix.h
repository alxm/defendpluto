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

typedef int16_t ZFix;
typedef int32_t ZFixBig;

#define Z_FIX_BIT_PRECISION (8)
#define Z_FIX_ONE           (1 << Z_FIX_BIT_PRECISION)
#define Z_FIX_NUM_ANGLES    (256)

extern ZFix z_fix__sin[Z_FIX_NUM_ANGLES];

static inline ZFix z_fix_itofix(int8_t X)
{
    return (ZFix)(X << Z_FIX_BIT_PRECISION);
}

static inline int8_t z_fix_fixtoi(ZFix X)
{
    return (int8_t)(X >> Z_FIX_BIT_PRECISION);
}

static inline ZFix z_fix_mul(ZFix X, ZFix Y)
{
    return (ZFix)(((ZFixBig)X * Y) >> Z_FIX_BIT_PRECISION);
}

static inline ZFix z_fix_div(ZFix X, ZFix Y)
{
    return (ZFix)(((ZFixBig)X << Z_FIX_BIT_PRECISION) / Y);
}

static inline ZFix z_fix_sin(uint8_t Angle)
{
    return z_fix__sin[Angle];
}

static inline ZFix z_fix_cos(uint8_t Angle)
{
    return z_fix__sin[(uint8_t)(Angle + 64)];
}

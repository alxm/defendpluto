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

typedef int32_t ZFixBig;

#define Z_FIX_BIT_PRECISION (7)
#define Z_FIX_ONE           (1 << Z_FIX_BIT_PRECISION)
#define Z_FIX_NUM_ANGLES    (256)

typedef enum {
    Z_FIX_ANGLE_000 = (Z_FIX_ONE / 8 * Z_FIX_NUM_ANGLES * 0),
    Z_FIX_ANGLE_045 = (Z_FIX_ONE / 8 * Z_FIX_NUM_ANGLES * 1),
    Z_FIX_ANGLE_090 = (Z_FIX_ONE / 8 * Z_FIX_NUM_ANGLES * 2),
    Z_FIX_ANGLE_135 = (Z_FIX_ONE / 8 * Z_FIX_NUM_ANGLES * 3),
    Z_FIX_ANGLE_180 = (Z_FIX_ONE / 8 * Z_FIX_NUM_ANGLES * 4),
    Z_FIX_ANGLE_225 = (Z_FIX_ONE / 8 * Z_FIX_NUM_ANGLES * 5),
    Z_FIX_ANGLE_270 = (Z_FIX_ONE / 8 * Z_FIX_NUM_ANGLES * 6),
    Z_FIX_ANGLE_315 = (Z_FIX_ONE / 8 * Z_FIX_NUM_ANGLES * 7)
} ZFixAngle;

extern const ZFix z_fix__sin[Z_FIX_NUM_ANGLES];

static inline ZFix z_fix_itofix(int8_t X)
{
    return zf(X << Z_FIX_BIT_PRECISION);
}

static inline int8_t z_fix_fixtoi(ZFix X)
{
    return i8(X >> Z_FIX_BIT_PRECISION);
}

static inline ZFix z_fix_mul(ZFix X, ZFix Y)
{
    return zf(((ZFixBig)X * Y) >> Z_FIX_BIT_PRECISION);
}

static inline ZFix z_fix_div(ZFix X, ZFix Y)
{
    return zf(((ZFixBig)X << Z_FIX_BIT_PRECISION) / Y);
}

static inline ZFix z_fix_sin(uint8_t Angle)
{
    return Z_PGM_READ_UINT16(z_fix__sin[Angle]);
}

static inline ZFix z_fix_cos(uint8_t Angle)
{
    return Z_PGM_READ_UINT16(z_fix__sin[u8(Angle + 64)]);
}

#define Z_GEN_MINMAX(Type, Name)                                    \
    static inline Type z_##Name##_min(Type X, Type Y)               \
    {                                                               \
        return X < Y ? X : Y;                                       \
    }                                                               \
                                                                    \
    static inline Type z_##Name##_max(Type X, Type Y)               \
    {                                                               \
        return X > Y ? X : Y;                                       \
    }                                                               \
                                                                    \
    static inline Type z_##Name##_clamp(Type X, Type Min, Type Max) \
    {                                                               \
        return X < Min ? Min : (X > Max ? Max : X);                 \
    }

Z_GEN_MINMAX(int16_t, int16)
Z_GEN_MINMAX(ZFix, fix)

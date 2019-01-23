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

typedef int16_t ZFix;
typedef int32_t ZFixBig;

#define Z_FIX_BIT_PRECISION (7)
#define Z_FIX_ONE           (1 << Z_FIX_BIT_PRECISION)

#define Z_ANGLES_NUM        (128)

#define Z_ANGLE_WRAP(Angle) u8((Angle) & (Z_ANGLES_NUM - 1))
#define Z_ANGLE_TO_U4(Angle) u4((Angle) >> 3)
#define Z_U4_TO_ANGLE(U4) u8((U4) << 3)

typedef enum {
    Z_ANGLE_000 = (Z_ANGLES_NUM * 0 / 8),
    Z_ANGLE_022 = (Z_ANGLES_NUM * 1 / 16),
    Z_ANGLE_045 = (Z_ANGLES_NUM * 1 / 8),
    Z_ANGLE_067 = (Z_ANGLE_045 + Z_ANGLE_022),
    Z_ANGLE_090 = (Z_ANGLES_NUM * 2 / 8),
    Z_ANGLE_112 = (Z_ANGLE_090 + Z_ANGLE_022),
    Z_ANGLE_135 = (Z_ANGLES_NUM * 3 / 8),
    Z_ANGLE_157 = (Z_ANGLE_135 + Z_ANGLE_022),
    Z_ANGLE_180 = (Z_ANGLES_NUM * 4 / 8),
    Z_ANGLE_202 = (Z_ANGLE_180 + Z_ANGLE_022),
    Z_ANGLE_225 = (Z_ANGLES_NUM * 5 / 8),
    Z_ANGLE_247 = (Z_ANGLE_225 + Z_ANGLE_022),
    Z_ANGLE_270 = (Z_ANGLES_NUM * 6 / 8),
    Z_ANGLE_292 = (Z_ANGLE_270 + Z_ANGLE_022),
    Z_ANGLE_315 = (Z_ANGLES_NUM * 7 / 8),
    Z_ANGLE_337 = (Z_ANGLE_315 + Z_ANGLE_022)
} ZAngle;

extern const ZFix z_fix__sin[Z_ANGLES_NUM];

extern uint8_t z_fix_atan(ZFix X1, ZFix Y1, ZFix X2, ZFix Y2);

static inline ZFix z_fix_fromInt(int16_t X)
{
    return zf(X << Z_FIX_BIT_PRECISION);
}

static inline int16_t z_fix_toInt(ZFix X)
{
    return i16(X >> Z_FIX_BIT_PRECISION);
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
    return z_pgm_readI16(z_fix__sin + Angle);
}

static inline ZFix z_fix_cos(uint8_t Angle)
{
    return z_pgm_readI16(
        z_fix__sin + Z_ANGLE_WRAP(Angle + Z_ANGLES_NUM / 4));
}

#define Z_GEN_MIN(Type, Name)                       \
    static inline Type z_min_##Name(Type X, Type Y) \
    {                                               \
        return X < Y ? X : Y;                       \
    }

#define Z_GEN_MAX(Type, Name)                       \
    static inline Type z_max_##Name(Type X, Type Y) \
    {                                               \
        return X > Y ? X : Y;                       \
    }

#define Z_GEN_ABS(Type, Name)               \
    static inline Type z_abs_##Name(Type X) \
    {                                       \
        return X < 0 ? (Type)-X : X;        \
    }

#define Z_GEN_CLA(Type, Name)                                     \
    static inline Type z_clamp_##Name(Type X, Type Min, Type Max) \
    {                                                             \
        return X < Min ? Min : (X > Max ? Max : X);               \
    }

#define Z_GEN_RND(Type, Name)                    \
    static inline Type z_random_##Name(Type Max) \
    {                                            \
        return (Type)(rand() % Max);             \
    }

Z_GEN_MIN(int16_t, int16)
Z_GEN_MIN(ZFix, fix)

Z_GEN_MAX(int16_t, int16)
Z_GEN_MAX(ZFix, fix)

Z_GEN_ABS(ZFix, fix)

Z_GEN_CLA(int16_t, int16)
Z_GEN_CLA(ZFix, fix)

Z_GEN_RND(int8_t, int8)
Z_GEN_RND(uint8_t, uint8)
Z_GEN_RND(int16_t, int16)

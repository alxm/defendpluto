/*
    Copyright 2017 Alex Margarit <alex@alxm.org>
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

#include "platform.h"
#include "util_fix.h"

PROGMEM static const uint8_t g_atanAngles[Z_FIX_ONE] = {
    0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2,
    2, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 5, 5,
    5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7,
    7, 7, 7, 8, 8, 8, 8, 8, 8, 8, 9, 9, 9, 9, 9, 9,
    9, 9, 10, 10, 10, 10, 10, 10, 10, 10, 11, 11, 11, 11, 11, 11,
    11, 11, 11, 12, 12, 12, 12, 12, 12, 12, 12, 12, 13, 13, 13, 13,
    13, 13, 13, 13, 13, 13, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
    14, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 16, 16, 16,
};

PROGMEM const ZFix z_fix__sin[Z_ANGLES_NUM] = {
    0, 6, 12, 18, 24, 31, 37, 43,
    48, 54, 60, 65, 71, 76, 81, 85,
    90, 94, 98, 102, 106, 109, 112, 115,
    118, 120, 122, 124, 125, 126, 127, 127,
    128, 127, 127, 126, 125, 124, 122, 120,
    118, 115, 112, 109, 106, 102, 98, 94,
    90, 85, 81, 76, 71, 65, 60, 54,
    48, 43, 37, 31, 24, 18, 12, 6,
    0, -6, -12, -18, -24, -31, -37, -43,
    -48, -54, -60, -65, -71, -76, -81, -85,
    -90, -94, -98, -102, -106, -109, -112, -115,
    -118, -120, -122, -124, -125, -126, -127, -127,
    -128, -127, -127, -126, -125, -124, -122, -120,
    -118, -115, -112, -109, -106, -102, -98, -94,
    -90, -85, -81, -76, -71, -65, -60, -54,
    -48, -43, -37, -31, -24, -18, -12, -6,
};

uint8_t z_fix_atan(ZFix X1, ZFix Y1, ZFix X2, ZFix Y2)
{
    const ZFix dx = z_abs_fix(zf(X2 - X1));
    const ZFix dy = z_abs_fix(zf(Y2 - Y1));

    if(dx == dy) {
        if(X2 >= X1) {
            if(Y2 <= Y1) {
                return Z_ANGLE_045;
            } else {
                return Z_ANGLE_315;
            }
        } else {
            if(Y2 <= Y1) {
                return Z_ANGLE_135;
            } else {
                return Z_ANGLE_225;
            }
        }
    } else if(dx == 0) {
        if(Y2 <= Y1) {
            return Z_ANGLE_090;
        } else {
            return Z_ANGLE_270;
        }
    } else if(dy == 0) {
        if(X2 >= X1) {
            return 0;
        } else {
            return Z_ANGLE_180;
        }
    }

    const ZFix ratio = z_fix_div(z_min_fix(dx, dy), z_max_fix(dx, dy));
    const uint8_t cachedAngle = z_pgm_readU8(g_atanAngles + ratio);

    if(dy < dx) {
        if(X2 >= X1) {
            if(Y2 <= Y1) {
                return cachedAngle;
            } else {
                return Z_ANGLE_WRAP(-cachedAngle);
            }
        } else {
            if(Y2 <= Y1) {
                return u8(Z_ANGLE_180 - cachedAngle);
            } else {
                return u8(Z_ANGLE_180 + cachedAngle);
            }
        }
    } else {
        if(X2 >= X1) {
            if(Y2 <= Y1) {
                return u8(Z_ANGLE_090 - cachedAngle);
            } else {
                return u8(Z_ANGLE_270 + cachedAngle);
            }
        } else {
            if(Y2 <= Y1) {
                return u8(Z_ANGLE_090 + cachedAngle);
            } else {
                return u8(Z_ANGLE_270 - cachedAngle);
            }
        }
    }
}

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

#include "platform.h"
#include "util_fix.h"

PROGMEM const ZFix z_fix__sin[Z_FIX_NUM_ANGLES] = {
    0, 3, 6, 9, 12, 15, 18, 21, 24,
    28, 31, 34, 37, 40, 43, 46, 48,
    51, 54, 57, 60, 63, 65, 68, 71,
    73, 76, 78, 81, 83, 85, 88, 90,
    92, 94, 96, 98, 100, 102, 104, 106,
    108, 109, 111, 112, 114, 115, 117, 118,
    119, 120, 121, 122, 123, 124, 124, 125,
    126, 126, 127, 127, 127, 127, 127, 128,
    127, 127, 127, 127, 127, 126, 126, 125,
    124, 124, 123, 122, 121, 120, 119, 118,
    117, 115, 114, 112, 111, 109, 108, 106,
    104, 102, 100, 98, 96, 94, 92, 90,
    88, 85, 83, 81, 78, 76, 73, 71,
    68, 65, 63, 60, 57, 54, 51, 48,
    46, 43, 40, 37, 34, 31, 28, 24,
    21, 18, 15, 12, 9, 6, 3, 0,
    -3, -6, -9, -13, -16, -19, -22, -25,
    -28, -31, -34, -37, -40, -43, -46, -49,
    -52, -55, -58, -60, -63, -66, -68, -71,
    -74, -76, -79, -81, -84, -86, -88, -91,
    -93, -95, -97, -99, -101, -103, -105, -106,
    -108, -110, -111, -113, -114, -116, -117, -118,
    -119, -121, -122, -122, -123, -124, -125, -126,
    -126, -127, -127, -127, -128, -128, -128, -128,
    -128, -128, -128, -127, -127, -127, -126, -126,
    -125, -124, -123, -122, -122, -121, -119, -118,
    -117, -116, -114, -113, -111, -110, -108, -106,
    -105, -103, -101, -99, -97, -95, -93, -91,
    -88, -86, -84, -81, -79, -76, -74, -71,
    -68, -66, -63, -60, -58, -55, -52, -49,
    -46, -43, -40, -37, -34, -31, -28, -25,
    -22, -19, -16, -13, -9, -6, -3,
};

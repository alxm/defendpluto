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

PROGMEM const ZFix z_fix__sin[Z_ANGLES_NUM] = {
    0, 6, 12, 18, 24,
    31, 37, 43, 48,
    54, 60, 65, 71,
    76, 81, 85, 90,
    94, 98, 102, 106,
    109, 112, 115, 118,
    120, 122, 124, 125,
    126, 127, 127, 128,
    127, 127, 126, 125,
    124, 122, 120, 118,
    115, 112, 109, 106,
    102, 98, 94, 90,
    85, 81, 76, 71,
    65, 60, 54, 48,
    43, 37, 31, 24,
    18, 12, 6, 0,
    -6, -12, -18, -24,
    -31, -37, -43, -48,
    -54, -60, -65, -71,
    -76, -81, -85, -90,
    -94, -98, -102, -106,
    -109, -112, -115, -118,
    -120, -122, -124, -125,
    -126, -127, -127, -128,
    -127, -127, -126, -125,
    -124, -122, -120, -118,
    -115, -112, -109, -106,
    -102, -98, -94, -90,
    -85, -81, -76, -71,
    -65, -60, -54, -48,
    -43, -37, -31, -24,
    -18, -12, -6,
};

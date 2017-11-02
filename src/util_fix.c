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
    0, 6, 12, 18, 25, 31, 37, 43, 49,
    56, 62, 68, 74, 80, 86, 92, 97,
    103, 109, 115, 120, 126, 131, 136, 142,
    147, 152, 157, 162, 167, 171, 176, 181,
    185, 189, 193, 197, 201, 205, 209, 212,
    216, 219, 222, 225, 228, 231, 234, 236,
    238, 241, 243, 244, 246, 248, 249, 251,
    252, 253, 254, 254, 255, 255, 255, 256,
    255, 255, 255, 254, 254, 253, 252, 251,
    249, 248, 246, 244, 243, 241, 238, 236,
    234, 231, 228, 225, 222, 219, 216, 212,
    209, 205, 201, 197, 193, 189, 185, 181,
    176, 171, 167, 162, 157, 152, 147, 142,
    136, 131, 126, 120, 115, 109, 103, 97,
    92, 86, 80, 74, 68, 62, 56, 49,
    43, 37, 31, 25, 18, 12, 6, 0,
    -6, -12, -18, -25, -31, -37, -43, -49,
    -56, -62, -68, -74, -80, -86, -92, -97,
    -103, -109, -115, -120, -126, -131, -136, -142,
    -147, -152, -157, -162, -167, -171, -176, -181,
    -185, -189, -193, -197, -201, -205, -209, -212,
    -216, -219, -222, -225, -228, -231, -234, -236,
    -238, -241, -243, -244, -246, -248, -249, -251,
    -252, -253, -254, -254, -255, -255, -255, -256,
    -255, -255, -255, -254, -254, -253, -252, -251,
    -249, -248, -246, -244, -243, -241, -238, -236,
    -234, -231, -228, -225, -222, -219, -216, -212,
    -209, -205, -201, -197, -193, -189, -185, -181,
    -176, -171, -167, -162, -157, -152, -147, -142,
    -136, -131, -126, -120, -115, -109, -103, -97,
    -92, -86, -80, -74, -68, -62, -56, -49,
    -43, -37, -31, -25, -18, -12, -6,
};

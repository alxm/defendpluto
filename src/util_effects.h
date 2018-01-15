/*
    Copyright 2018 Alex Margarit <alex@alxm.org>

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

typedef enum {
    Z_LIGHT_INVALID = -1,
    Z_LIGHT_EXPLOSION,
    Z_LIGHT_PLAYER_SHOOTING,
    Z_LIGHT_NUM
} ZLightId;

extern void z_effect_particles(ZFix X, ZFix Y, uint8_t Num);
extern void z_effect_circles(ZFix X, ZFix Y);
extern void z_effect_light(ZFix X, ZFix Y, uint8_t LightId);

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

typedef struct {
    ZPoolObject poolObject;
    ZFix x, y;
    uint8_t angle : 7;
    uint8_t splitNum : 2;
    uint8_t ticks : 7;
} ZParticle;

extern void z_particle_init(ZParticle* Particle, ZFix X, ZFix Y);
extern ZPoolTickCallback z_particle_tick;
extern ZPoolDrawCallback z_particle_draw;

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

#include "platform.h"
#include "util_fix.h"
#include "util_pool.h"
#include "obj_particle.h"

void z_effect_particles(ZFix X, ZFix Y, uint8_t Num)
{
    while(Num--) {
        ZParticle* p = z_pool_alloc(Z_POOL_PARTICLE);

        if(p == NULL) {
            return;
        }

        z_particle_init(p, X, Y);
    }
}

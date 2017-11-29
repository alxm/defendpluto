/*
    Copyright 2017 Alex Margarit <alex@alxm.org>

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
#include "util_graphics.h"
#include "util_pool.h"
#include "util_random.h"
#include "util_screen.h"
#include "obj_particle.h"

void z_particle_init(ZParticle* Particle, ZFix X, ZFix Y)
{
    Particle->x = X;
    Particle->y = Y;
    Particle->angle = Z_ANGLE_TO_U4(z_random_uint8(Z_ANGLES_NUM));
    Particle->ttl = u4(z_random_uint8(16));
}

bool z_particle_tick(ZPoolObject* Particle)
{
    ZParticle* particle = (ZParticle*)Particle;
    uint8_t angle = Z_U4_TO_ANGLE(particle->angle);

    particle->x = zf(particle->x + z_fix_cos(angle));
    particle->y = zf(particle->y - z_fix_sin(angle));

    return particle->ttl--;
}

void z_particle_draw(ZPoolObject* Particle)
{
    ZParticle* particle = (ZParticle*)Particle;

    int16_t x = i16(z_fix_fixtoi(particle->x) + z_screen_getXShake());
    int16_t y = i16(z_fix_fixtoi(particle->y) + z_screen_getYShake());

    z_draw_pixel(x, y, Z_COLOR_YELLOW);
}

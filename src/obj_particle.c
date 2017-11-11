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
#include "util_graphics.h"
#include "util_pool.h"
#include "util_random.h"
#include "util_screen.h"
#include "obj_particle.h"

void z_particle_init(ZParticle* Particle, ZFix X, ZFix Y, uint8_t FramesTtl)
{
    Particle->x = X;
    Particle->y = Y;
    Particle->angle = u8(z_random_uint16(Z_FIX_NUM_ANGLES));
    Particle->ttl = FramesTtl;
}

bool z_particle_tick(ZPoolObject* Particle)
{
    ZParticle* particle = (ZParticle*)Particle;

    particle->x = z_fix_inc(particle->x, z_fix_cos(particle->angle));
    particle->y = z_fix_inc(particle->y, zf(-z_fix_sin(particle->angle)));

    return particle->ttl--;
}

void z_particle_draw(ZPoolObject* Particle)
{
    ZParticle* particle = (ZParticle*)Particle;

    int8_t x = i8(z_fix_fixtoi(particle->x) + z_screen_xShake);
    int8_t y = i8(z_fix_fixtoi(particle->y) + z_screen_yShake);

    z_draw_pixel(x, y, Z_COLOR_YELLOW);
}

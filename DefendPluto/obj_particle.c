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

#include "platform.h"
#include "obj_particle.h"

#include "util_fps.h"
#include "util_screen.h"

struct ZParticle {
    ZPoolObjHeader poolObject;
    ZFix x, y;
    uint8_t angle : 7;
    uint8_t splitNum : 2;
    uint8_t ticks : 7;
};

Z_POOL_DECLARE(ZParticle, 24, g_pool);

void z_particle_setup(void)
{
    z_pool_register(Z_POOL_PARTICLE, g_pool);
}

void z_particle_init(ZParticle* Particle, ZFix X, ZFix Y)
{
    Particle->x = X;
    Particle->y = Y;
    Particle->angle = Z_ANGLE_WRAP(z_random_uint8(Z_ANGLES_NUM));
    Particle->splitNum = u2(z_random_uint8(4));
    Particle->ticks = 0;
}

bool z_particle_tick(ZPoolObjHeader* Particle, void* Context)
{
    Z_UNUSED(Context);

    ZParticle* particle = (ZParticle*)Particle;

    particle->ticks++;

    return particle->ticks < Z_FPS / 4
        || (particle->ticks < Z_FPS && z_random_int8(4) != 0);
}

void z_particle_draw(ZPoolObjHeader* Particle)
{
    ZParticle* particle = (ZParticle*)Particle;

    uint8_t splitNum = u8(1 + particle->splitNum);
    uint8_t angle = particle->angle;

    static const uint8_t incs[4] = {
        Z_ANGLE_112, Z_ANGLE_090, Z_ANGLE_067, Z_ANGLE_090
    };

    for(uint8_t i = 0; i < splitNum; i++) {
        int16_t x = i16(
            z_fix_toInt(zf(particle->x + z_fix_cos(angle) * particle->ticks))
                + z_screen_getXShake());
        int16_t y = i16(
            z_fix_toInt(zf(particle->y - z_fix_sin(angle) * particle->ticks))
                + z_screen_getYShake());

        z_draw_pixel(x,
                     y,
                     z_fps_getCounter() & 2 ? Z_COLOR_RED : Z_COLOR_YELLOW);

        angle = Z_ANGLE_WRAP(angle + incs[i]);
    }
}

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
#include "util_effects.h"

#include "obj_circle.h"
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

void z_effect_circles(ZFix X, ZFix Y)
{
    ZCircle* c = z_pool_alloc(Z_POOL_CIRCLE);

    if(c) {
        z_circle_init(c, X, Y);
    }
}

void z_effect_light(ZFix X, ZFix Y, ZLightId LightId)
{
    #if Z_PLATFORM_GAMEBUINOMETA
        int16_t x = z_fix_fixtoi(X);
        int16_t y = z_fix_fixtoi(Y);

        x = i16(2 * z_int16_clamp(x, 0, i16(Z_SCREEN_W - 1)) / Z_SCREEN_W);
        y = i16(4 * z_int16_clamp(y, 0, i16(Z_SCREEN_H - 1)) / Z_SCREEN_H);

        uint8_t color;

        switch(LightId) {
            case Z_LIGHT_EXPLOSION: {
                color = Z_COLOR_RED;
            } break;

            case Z_LIGHT_PLAYER_SHOOTING: {
                color = Z_COLOR_YELLOW;
            } break;

            default: {
                color = 0;
            } break;
        }

        z_lights_put(x, y, color);
    #else
        Z_UNUSED(X);
        Z_UNUSED(Y);
        Z_UNUSED(LightId);
    #endif
}

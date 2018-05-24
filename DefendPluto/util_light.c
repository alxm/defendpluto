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
#include "util_light.h"

#include "util_fix.h"
#include "util_fps.h"

static const struct {
    ZColorId color;
    uint8_t counterSpeed[2];
} g_patterns[Z_LIGHT_NUM] = {
    [Z_LIGHT_EXPLOSION] = {Z_COLOR_RED, {8, 2}},
    [Z_LIGHT_PLAYER_SHOOTING] = {Z_COLOR_YELLOW, {16, 8}},
};

static struct {
    ZLightId id;
    uint8_t counter;
} g_light;

static void z_light_fill(ZColorId Color, int Intensity)
{
    #if Z_PLATFORM_META
        z_platform_meta_fillLights(Color, Intensity);
    #else
        Z_UNUSED(Color);
        Z_UNUSED(Intensity);
    #endif
}

void z_light_reset(void)
{
    g_light.id = Z_LIGHT_INVALID;
    g_light.counter = 0;
}

void z_light_tick(void)
{
    if(g_light.id == Z_LIGHT_INVALID) {
        return;
    }

    g_light.counter = u8(g_light.counter
        + g_patterns[g_light.id].counterSpeed[g_light.counter >= Z_ANGLE_090]);

    if(g_light.counter >= Z_ANGLE_180) {
        z_light_reset();

        #if Z_PLATFORM_META
            z_platform_meta_fillLights(Z_COLOR_INVALID, 0);
        #endif
    } else {
        z_light_fill(g_patterns[g_light.id].color,
                     z_fix_toInt(zf(z_fix_sin(g_light.counter) * 255)));
    }
}

void z_light_start(ZLightId Light)
{
    g_light.id = Light;
    g_light.counter = 0;
}

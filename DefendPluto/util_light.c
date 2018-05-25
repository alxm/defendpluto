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
    ZColorId bgColor;
    ZLightId pulseId;
    uint8_t counter;
} g_light;

void z_light_reset(void)
{
    g_light.bgColor = Z_COLOR_INVALID;
    g_light.pulseId = Z_LIGHT_INVALID;
    g_light.counter = 0;
}

void z_light_tick(void)
{
    ZColorId color = Z_COLOR_INVALID;
    int alpha = 0;

    if(g_light.pulseId != Z_LIGHT_INVALID) {
        bool goingDown = g_light.counter >= Z_ANGLE_090;
        uint8_t speed = g_patterns[g_light.pulseId].counterSpeed[goingDown];
        g_light.counter = u8(g_light.counter + speed);

        if(g_light.counter >= Z_ANGLE_180) {
            g_light.pulseId = Z_LIGHT_INVALID;
        } else {
            color = g_patterns[g_light.pulseId].color;
            alpha = z_fix_toInt(zf(z_fix_sin(g_light.counter) * 255));
        }
    }

    #if Z_PLATFORM_META
        z_platform_meta_fillLights(g_light.bgColor, color, alpha);
    #else
        Z_UNUSED(alpha);
    #endif
}

void z_light_setPulse(ZLightId Light)
{
    g_light.pulseId = Light;
    g_light.counter = 0;
}

void z_light_setBackground(ZColorId ColorId)
{
    g_light.bgColor = ColorId;
}

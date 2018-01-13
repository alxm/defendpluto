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

#ifndef ARDUINO

#include <a2x.h>
#include "platform.h"
#include "loop.h"
#include "util_fix.h"
#include "util_fps.h"
#include "util_screen.h"

static const char* intToString(int X)
{
    static char buffer[4];
    snprintf(buffer, sizeof(buffer), "%d", X);

    return buffer;
}

A_SETUP
{
    a_settings_set("app.title", "Defend Pluto");
    a_settings_set("app.version", "1.0");
    a_settings_set("app.author", "alxm");
    a_settings_set("app.output.on", "yes");
    a_settings_set("video.width", intToString(z_screen_w));
    a_settings_set("video.height", intToString(z_screen_h));
    a_settings_set("fps.tick", intToString(z_fps_rate));
    a_settings_set("fps.draw", intToString(z_fps_rate));
}

A_STATE(run)
{
    A_STATE_INIT
    {
        z_loop_setup();
    }

    A_STATE_TICK
    {
        z_loop_tick();
    }

    A_STATE_DRAW
    {
        z_loop_draw();
    }
}

A_MAIN
{
    #if Z_DEBUG_GENERATE_LUT
        {
            uint8_t angle = 0;
            ZFix lastRatio = 0;

            printf("PROGMEM static const uint8_t g_atanAngles[Z_FIX_ONE] = {");

            for(ZFix refRatio = 0; refRatio < Z_FIX_ONE; refRatio++) {
                if(refRatio % 16 == 0) {
                    printf("\n    ");
                }

                ZFix ratio = zf(a_fix_div(a_fix_sin(angle), a_fix_cos(angle))
                                    / (A_FIX_ONE / Z_FIX_ONE));

                while(ratio < refRatio) {
                    angle++;
                    lastRatio = ratio;
                    ratio = zf(a_fix_div(a_fix_sin(angle), a_fix_cos(angle))
                                    / (A_FIX_ONE / Z_FIX_ONE));
                }

                ZFix diff1 = zf(refRatio - lastRatio);
                ZFix diff2 = zf(ratio - refRatio);

                if(diff2 <= diff1) {
                    printf("%d, ",
                           angle / (A_MATH_ANGLES_NUM / Z_ANGLES_NUM));
                } else {
                    printf("%d, ",
                           (unsigned)
                            (angle - 1) / (A_MATH_ANGLES_NUM / Z_ANGLES_NUM));
                }
            }

            printf("\n};\n");
        }

        printf("\n");

        {
            printf("PROGMEM const ZFix z_fix__sin[Z_ANGLES_NUM] = {");

            for(unsigned a = 0;
                a < A_MATH_ANGLES_NUM;
                a += A_MATH_ANGLES_NUM / Z_ANGLES_NUM) {

                if(a % 16 == 0) {
                    printf("\n    ");
                }

                printf("%d, ", a_fix_sin(a) / (A_FIX_ONE / Z_FIX_ONE));
            }

            printf("\n};\n");
        }
    #else
        a_state_new("run", run, "", "");
        a_state_push("run");
    #endif
}

#endif // ifndef ARDUINO

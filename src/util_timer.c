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
#include "util_fps.h"
#include "util_timer.h"

typedef struct {
    uint8_t base;
    uint8_t period : 7;
    bool expired : 1;
} ZTimer;

static ZTimer g_timers[Z_TIMER_NUM];

void z_timer_tick(void)
{
    uint8_t now = u8(z_fps_getCounter());

    for(uint8_t t = 0; t < Z_TIMER_NUM; t++) {
        ZTimer* timer = &g_timers[t];

        if(u8(now - timer->base) >= timer->period) {
            timer->base = now;
            timer->expired = true;
        } else {
            timer->expired = false;
        }
    }
}

void z_timer_start(uint8_t Timer, uint8_t Ds)
{
    ZTimer* timer = &g_timers[Timer];

    timer->base = u8(z_fps_getCounter());
    timer->period = uN(Z_DS_TO_FRAMES(Ds), 7);
    timer->expired = false;
}

bool z_timer_expired(uint8_t Timer)
{
    return g_timers[Timer].expired;
}

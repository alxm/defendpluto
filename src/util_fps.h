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

Z_EXTERN_C_START

static const uint8_t Z_FPS_RATE = 30;

extern uint16_t z_fps_getCounter(void);
extern bool z_fps_isNthFrame(uint8_t N);

static inline uint8_t z_fps_dsToTicks(uint8_t Ds)
{
    return u8(Z_FPS_RATE * Ds / 10);
}

Z_EXTERN_C_END

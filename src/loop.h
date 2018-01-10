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

typedef enum {
    Z_STATE_INVALID = -1,
    Z_STATE_DIED,
    Z_STATE_DOORS_CLOSE,
    Z_STATE_DOORS_OPEN,
    Z_STATE_NEW,
    Z_STATE_NEXT,
    Z_STATE_OVER,
    Z_STATE_PLAY,
    Z_STATE_TITLE,
    Z_STATE_NUM
} ZStateId;

typedef void (ZStateCallback)(void);

extern void z_loop_setup(void);
extern void z_loop_tick(void);
extern void z_loop_draw(void);

extern int8_t z_loop_getLastState(void);
extern void z_loop_setState(int8_t State);

Z_EXTERN_C_END

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

#pragma once

typedef enum Z_ENUM_PACK {
    Z_TIMER_INVALID = -1,
    Z_TIMER_ENEMY_FRAME,
    Z_TIMER_HUD_HEARTS,
    Z_TIMER_HUD_SCORE,
    Z_TIMER_PLAYER_INVINCIBLE,
    Z_TIMER_PLAYER_REGEN_ENERGY,
    Z_TIMER_PLAYER_REGEN_SHIELD,
    Z_TIMER_PLAYER_SHOOT,
    Z_TIMER_G1,
    Z_TIMER_G2,
    Z_TIMER_NUM
} ZTimerId;

extern void z_timer_tick(void);

extern void z_timer_start(ZTimerId Timer, uint8_t Ds);
extern bool z_timer_expired(ZTimerId Timer);

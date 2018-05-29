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

#pragma once

#include "util_fix.h"

#define Z_PLAYER_MAX_HEALTH (3)
#define Z_PLAYER_MAX_SHIELD (15)
#define Z_PLAYER_MAX_ENERGY (15)

#define Z_PLAYER_W_NORMAL   (10)
#define Z_PLAYER_H_NORMAL   (4)
#define Z_PLAYER_W_SHIELD   (16)
#define Z_PLAYER_H_SHIELD   (16)

typedef enum Z_ENUM_PACK {
    Z_POINTS_INVALID = -1,
    Z_POINTS_ENEMY_HIT = 10,
    Z_POINTS_ENEMY_DESTROYED = 50,
    Z_POINTS_ENEMY_SMASHED = 25,
    Z_POINTS_NUM
} ZScorePoints;

extern void z_player_init(void);
extern void z_player_tick(void);
extern void z_player_draw(void);

extern void z_player_resetPosition(void);
extern void z_player_takeDamage(uint8_t Damage);
extern void z_player_scorePoints(uint8_t Points);

extern uint8_t z_player_getLevel(void);
extern void z_player_setLevel(uint8_t Level);
extern uint16_t z_player_getScore(void);
extern uint8_t z_player_getEnergy(void);
extern uint8_t z_player_getShield(void);
extern int8_t z_player_getHealth(void);
extern void z_player_getCoords(ZFix* X, ZFix* Y);

extern bool z_player_checkCollision(ZFix X, ZFix Y, int8_t W, int8_t H, uint8_t Damage);

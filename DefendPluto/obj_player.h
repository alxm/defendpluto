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

static const int8_t Z_PLAYER_MAX_HEALTH = 3;
static const uint8_t Z_PLAYER_MAX_SHIELD = 15;
static const uint8_t Z_PLAYER_MAX_ENERGY = 15;

static const int8_t Z_PLAYER_W_NORMAL = 10;
static const int8_t Z_PLAYER_H_NORMAL = 4;
static const int8_t Z_PLAYER_W_SHIELD = 16;
static const int8_t Z_PLAYER_H_SHIELD = 16;

typedef enum Z_ENUM_PACK {
    Z_POINTS_INVALID = -1,
    Z_POINTS_ENEMY_HIT = 10,
    Z_POINTS_ENEMY_DESTROYED = 50,
    Z_POINTS_ENEMY_SMASHED = 25,
    Z_POINTS_NUM
} ZScorePoints;

typedef struct {
    ZFix x, y;
    int16_t dx, dy;
    uint8_t frame : 4;
    uint8_t energy : 4;
    uint8_t shield : 4;
    int8_t health : 4;
    uint8_t lastShotCounter : 5;
    bool heartsBlink : 1;
    uint8_t shootShift : 1;
    bool jetFlicker : 1;
    uint8_t damage : 3;
    bool invincible : 1;
    uint16_t score;
    uint16_t scoreShow;
    uint8_t level;
} ZPlayer;

extern ZPlayer z_player;

extern void z_player_init(void);
extern void z_player_tick(bool CheckInput);
extern void z_player_draw(void);

extern void z_player_resetPosition(void);
extern void z_player_takeDamage(uint8_t Damage);
extern void z_player_scorePoints(uint8_t Points);

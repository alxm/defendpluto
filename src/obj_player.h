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

#define Z_HEALTH_MAX 3
#define Z_SHIELD_MAX 15
#define Z_ENERGY_MAX 15

typedef enum {
    Z_POINTS_INVALID = -1,
    Z_POINTS_ENEMY_HIT = 10,
    Z_POINTS_ENEMY_DESTROYED = 50,
    Z_POINTS_ENEMY_SMASHED = 25,
    Z_POINTS_NUM
} ZScorePoints;

typedef struct {
    ZFix x, y;
    int8_t dx, dy;
    uint8_t w : 4;
    uint8_t h : 4;
    uint8_t frame : 4;
    uint8_t energy : 4;
    uint8_t shield : 4;
    int8_t health : 4;
    uint8_t lastShotCounter : 5;
    bool heartsBlink : 1;
    uint8_t shootShift : 1;
    bool jetFlicker : 1;
    uint8_t damage : 3;
    uint8_t invincibleTimerDs : 5;
    uint16_t score;
    uint16_t scoreShow;
} ZPlayer;

extern ZPlayer z_player;

extern void z_player_init(void);
extern void z_player_tick(void);
extern void z_player_draw(void);

extern void z_player_takeDamage(uint8_t Damage);
extern void z_player_scorePoints(uint8_t Points);

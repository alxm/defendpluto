/*
    Copyright 2017 Alex Margarit <alex@alxm.org>

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

typedef enum {
    Z_ENEMY_INVALID = -1,
    Z_ENEMY_ASTEROID,
    Z_ENEMY_SHIP0,
    Z_ENEMY_SHIP1,
    Z_ENEMY_SHIP2,
    Z_ENEMY_NUM
} ZEnemyId;

typedef struct {
    ZSprite sprite;
    uint8_t w : 4;
    uint8_t h : 4;
    uint8_t health : 2;
    uint8_t damage : 3;
    uint8_t speedShift : 3;
} ZEnemyData;

extern ZEnemyData z_enemyData[Z_ENEMY_NUM];

extern void z_enemy_setup(void);

extern void z_enemy_drawJets(uint8_t EnemyId, int16_t X, int16_t Y);

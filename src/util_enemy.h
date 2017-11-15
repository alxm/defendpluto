/*
    Copyright 2017 Alex Margarit <alex@alxm.org>

    arduboy-shooter is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    arduboy-shooter is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with arduboy-shooter.  If not, see <http://www.gnu.org/licenses/>.
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
    uint8_t health : 4;
    uint8_t damage : 4;
    ZFix speed;
} ZEnemyData;

extern ZEnemyData z_enemyData[Z_ENEMY_NUM];

extern void z_enemy_setup(void);

extern void z_enemy_drawJets(uint8_t EnemyId, int8_t X, int8_t Y);

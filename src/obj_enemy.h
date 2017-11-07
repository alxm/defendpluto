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

typedef struct {
    ZPoolObject poolObject;
    ZFix x, y;
    uint8_t sprite, frame;
    uint8_t ai, aiData;
} ZEnemy;

#define Z_ENEMIES_NUM 16

extern void z_enemy_init(ZEnemy* Enemy, int8_t X, int8_t Y, uint8_t Sprite, uint8_t Ai, uint8_t AiData);
extern bool z_enemy_tick(ZPoolObject* Enemy);
extern void z_enemy_draw(ZPoolObject* Enemy);

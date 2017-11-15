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
    Z_AI_ID_INVALID = -1,
    Z_AI_DOWN,
    Z_AI_ZIGZAG,
    Z_AI_CURVE,
    Z_AI_ID_NUM
} ZAiId;

typedef struct {
    ZPoolObject poolObject;
    ZFix x, y;
    uint8_t angle : 7;
    bool jetFlicker : 1;
    uint8_t typeId : 4;
    uint8_t frame : 4;
    uint8_t aiId : 4;
    uint8_t aiState : 4;
    uint8_t aiArgs;
    uint8_t aiCounter1;
} ZEnemy;

#define Z_ENEMY_POOL_NUM 16

extern void z_enemy_init(ZEnemy* Enemy, int8_t X, int8_t Y, uint8_t TypeId, uint8_t AiId, uint8_t AiArgs);
extern bool z_enemy_tick(ZPoolObject* Enemy);
extern void z_enemy_draw(ZPoolObject* Enemy);

extern bool z_enemy_checkCollisions(int8_t X, int8_t Y, int8_t W, int8_t H, bool AllowMultipleCollisions);

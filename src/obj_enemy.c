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

#include "shared.h"
#include "util_pool.h"
#include "obj_enemy.h"

void z_enemy_init(ZEnemy* Enemy, int8_t X, int8_t Y)
{
    Enemy->x = X;
    Enemy->y = Y;
}

bool z_enemy_tick(ZPoolObject* Enemy)
{
    A_UNUSED(Enemy);

    return false;
}

void z_enemy_draw(ZPoolObject* Enemy)
{
    ZEnemy* enemy = (ZEnemy*)Enemy;

    int8_t x = enemy->x;
    int8_t y = enemy->y;

    s_draw_rectangle(x - 2, y - 2, 4, 4, true);
    s_draw_rectangle(x - 2 - 2, y - 2 - 1, 2, 8, true);
    s_draw_rectangle(x + 2, y - 2 - 1, 2, 8, true);
}

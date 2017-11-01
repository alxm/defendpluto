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
#include "util_fix.h"
#include "util_pool.h"
#include "obj_enemy.h"

static bool nobrain(ZEnemy* Enemy)
{
    Enemy->y = (ZFix)(Enemy->y + Z_FIX_ONE / 4);

    ZSprite sprite = z_gfx.enemy[Enemy->sprite];

    return z_fix_fixtoi(Enemy->y) - z_sprite_getHeight(sprite) / 2 < Z_HEIGHT;
}

static bool (*g_ai[])(ZEnemy*) = {
    nobrain
};

void z_enemy_init(ZEnemy* Enemy, int8_t X, int8_t Y, uint8_t Sprite, uint8_t Ai, uint8_t AiData)
{
    Enemy->x = z_fix_itofix(X);
    Enemy->y = z_fix_itofix(Y);
    Enemy->sprite = Sprite;
    Enemy->ai = Ai;
    Enemy->aiData = AiData;
}

bool z_enemy_tick(ZPoolObject* Enemy)
{
    ZEnemy* enemy = (ZEnemy*)Enemy;

    return g_ai[enemy->ai](enemy);
}

void z_enemy_draw(ZPoolObject* Enemy)
{
    ZEnemy* enemy = (ZEnemy*)Enemy;
    int8_t x = z_fix_fixtoi(enemy->x);
    int8_t y = z_fix_fixtoi(enemy->y);
    ZSprite sprite = z_gfx.enemy[enemy->sprite];

    z_sprite_blit(sprite,
                  (int8_t)(x - z_sprite_getWidth(sprite) / 2),
                  (int8_t)(y - z_sprite_getHeight(sprite) / 2),
                  Z_COLOR_YELLOW);
}

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

#include "platform.h"
#include "util_fix.h"
#include "util_graphics.h"
#include "util_pool.h"
#include "util_screen.h"
#include "obj_enemy.h"
#include "data_gfx_asteroid.h"
#include "data_gfx_enemy00.h"
#include "data_gfx_enemy01.h"
#include "data_gfx_enemy02.h"

ZEnemyData z_enemy_data[Z_ENEMY_NUM];

void z_enemy_setup(void)
{
    #define enemy(Index, Id, Width, Height, Health, Damage, Speed) \
        z_sprite_load(&z_enemy_data[Index].sprite, Id);            \
        z_enemy_data[Index].w = Width;                             \
        z_enemy_data[Index].h = Height;                            \
        z_enemy_data[Index].health = Health;                       \
        z_enemy_data[Index].damage = Damage;                       \
        z_enemy_data[Index].speedShift = Speed;

    enemy(Z_ENEMY_ASTEROID, asteroid, 8, 8, 1, 0, 2);
    enemy(Z_ENEMY_SHIP0, enemy00, 7, 5, 1, 2, 1);
    enemy(Z_ENEMY_SHIP1, enemy01, 7, 5, 1, 4, 1);
    enemy(Z_ENEMY_SHIP2, enemy02, 7, 6, 1, 6, 2);
}

void z_enemy_drawJets(uint8_t EnemyId, int16_t X, int16_t Y)
{
    int16_t x, y;
    int8_t w, h;

    switch(EnemyId) {
        case Z_ENEMY_SHIP0: {
            x = X;
            y = i16(Y - 2);
            z_draw_pixel(x, y, Z_COLOR_RED);
        } break;

        case Z_ENEMY_SHIP1: {
            x = i16(X - 3);
            y = i16(Y - 3);
            z_draw_pixel(x, y, Z_COLOR_RED);

            x = i16(X + 3);
            z_draw_pixel(x, y, Z_COLOR_RED);
        } break;

        case Z_ENEMY_SHIP2: {
            x = i16(X - 2);
            y = i16(Y - 4);
            w = 2;
            h = 1;
            z_draw_rectangle(x,
                             i16(y + z_screen_getYShake()),
                             w,
                             h,
                             Z_COLOR_RED);

            x = i16(X + 1);
            z_draw_rectangle(x,
                             i16(y + z_screen_getYShake()),
                             w,
                             h,
                             Z_COLOR_RED);
        } break;

        default: return;
    }
}

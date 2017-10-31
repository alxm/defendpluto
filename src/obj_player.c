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
#include "obj_bullet.h"
#include "obj_player.h"

#define SHOOT_EVERY_N_FRAMES (Z_FPS / 4)

ZPlayer z_player;

void z_player_init(int8_t X, int8_t Y)
{
    z_player.x = z_fix_itofix(X);
    z_player.y = z_fix_itofix(Y);
    z_player.lastShot = z_fps_getCounter();
}

void z_player_tick(void)
{
    if(z_button_pressed(z_controls.up) && z_player.y > 0) {
        z_player.y = (ZFix)(z_player.y - Z_FIX_ONE);
    } else if(z_button_pressed(z_controls.down) && z_fix_fixtoi(z_player.y) < Z_HEIGHT - 1) {
        z_player.y = (ZFix)(z_player.y + Z_FIX_ONE);
    }

    if(z_button_pressed(z_controls.left) && z_player.x > 0) {
        z_player.x = (ZFix)(z_player.x - Z_FIX_ONE);
    } else if(z_button_pressed(z_controls.right) && z_fix_fixtoi(z_player.x) < Z_WIDTH - 1) {
        z_player.x = (ZFix)(z_player.x + Z_FIX_ONE);
    }

    if(z_button_pressed(z_controls.a)) {
        if(z_fps_getCounter() - z_player.lastShot >= SHOOT_EVERY_N_FRAMES) {
            ZBullet* b = z_pool_alloc(z_pool[Z_POOL_BULLET]);

            if(b) {
                z_bullet_init(b, z_player.x, z_player.y, z_fix_itofix(-2));
            }

            z_player.lastShot = z_fps_getCounter();
        }
    } else {
        z_player.lastShot = (uint16_t)(z_fps_getCounter()
                                        - SHOOT_EVERY_N_FRAMES);
    }
}

void z_player_draw(void)
{
    int8_t x = z_fix_fixtoi(z_player.x);
    int8_t y = z_fix_fixtoi(z_player.y);

    z_draw_rectangle((int8_t)(x - 3), (int8_t)(y - 4), 6, 8, true);
}

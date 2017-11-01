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

#define Z_SHOOT_EVERY_N_FRAMES (Z_FPS / 4)
#define Z_SPEED_MAX (Z_FIX_ONE)
#define Z_SPEED_ACCEL (Z_FIX_ONE / 8)
#define Z_SPEED_DECEL (Z_FIX_ONE / 16)

ZPlayer z_player;

void z_player_init(int8_t X, int8_t Y)
{
    z_player.x = z_fix_itofix(X);
    z_player.y = z_fix_itofix(Y);
    z_player.dx = 0;
    z_player.dy = 0;
    z_player.lastShot = z_fps_getCounter();
    z_player.frame = 0;
    z_player.blink = false;
    z_player.jitter = false;
}

void z_player_tick(void)
{
    ZFix maxSpeed = Z_SPEED_MAX;

    if(z_button_pressed(z_controls.a)) {
        z_player.jitter = true;
        maxSpeed = Z_SPEED_MAX * 2 / 3;

        if(z_fps_getCounter() - z_player.lastShot >= Z_SHOOT_EVERY_N_FRAMES) {
            ZBullet* b = z_pool_alloc(z_pool[Z_POOL_BULLET]);

            if(b) {
                z_bullet_init(b, z_player.x, z_player.y, z_fix_itofix(-2));
            }

            z_player.lastShot = z_fps_getCounter();
        }
    } else {
        z_player.jitter = false;
        z_player.lastShot = (uint16_t)(z_fps_getCounter()
                                        - Z_SHOOT_EVERY_N_FRAMES);
    }

    if(z_button_pressed(z_controls.up)) {
        z_player.dy = (ZFix)(z_player.dy - Z_SPEED_ACCEL);
    } else if(z_button_pressed(z_controls.down)) {
        z_player.dy = (ZFix)(z_player.dy + Z_SPEED_ACCEL);
    } else {
        if(z_player.dy < 0) {
            z_player.dy = z_fix_min((ZFix)(z_player.dy + Z_SPEED_DECEL), 0);
        } else if(z_player.dy > 0) {
            z_player.dy = z_fix_max((ZFix)(z_player.dy - Z_SPEED_DECEL), 0);
        }
    }

    if(z_button_pressed(z_controls.left)) {
        z_player.frame = 1;
        z_player.dx = (ZFix)(z_player.dx - Z_SPEED_ACCEL);
    } else if(z_button_pressed(z_controls.right)) {
        z_player.frame = 2;
        z_player.dx = (ZFix)(z_player.dx + Z_SPEED_ACCEL);
    } else {
        z_player.frame = 0;

        if(z_player.dx < 0) {
            z_player.dx = z_fix_min((ZFix)(z_player.dx + Z_SPEED_DECEL), 0);
        } else if(z_player.dx > 0) {
            z_player.dx = z_fix_max((ZFix)(z_player.dx - Z_SPEED_DECEL), 0);
        }
    }

    z_player.dx = z_fix_clamp(z_player.dx, (ZFix)-maxSpeed, maxSpeed);
    z_player.dy = z_fix_clamp(z_player.dy, (ZFix)-maxSpeed, maxSpeed);

    z_player.x = z_fix_clamp(z_fix_inc(z_player.x, z_player.dx),
                             0,
                             z_fix_itofix(Z_WIDTH - 1));

    z_player.y = z_fix_clamp(z_fix_inc(z_player.y, z_player.dy),
                             0,
                             z_fix_itofix(Z_HEIGHT - 1));

    z_player.blink = !z_player.blink;
}

void z_player_draw(void)
{
    int8_t x = z_fix_fixtoi(z_player.x);
    int8_t y = z_fix_fixtoi(z_player.y);
    ZSprite sprite = z_gfx.player[z_player.frame];

    if(z_player.jitter) {
        y = (int8_t)(y + z_random_int8(2));
    }

    if(z_player.blink) {
        z_draw_rectangle((int8_t)(x - 3), (int8_t)(y + 4), 2, 1, Z_COLOR_RED);
        z_draw_rectangle((int8_t)(x + 1), (int8_t)(y + 4), 2, 1, Z_COLOR_RED);
    }

    z_sprite_blit(sprite,
                  (int8_t)(x - z_sprite_getWidth(sprite) / 2),
                  (int8_t)(y - z_sprite_getHeight(sprite) / 2),
                  Z_COLOR_YELLOW);
}

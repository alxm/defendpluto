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

#include "platform.h"
#include "util_fix.h"
#include "util_fps.h"
#include "util_graphics.h"
#include "util_input.h"
#include "util_pool.h"
#include "util_screen.h"
#include "obj_bulletp.h"
#include "obj_enemy.h"
#include "obj_player.h"

#define Z_HEALTH_MAX 3

#define Z_SHIELD_MAX 1024
#define Z_SHIELD_DAMAGE_COLLISION (Z_SHIELD_MAX / 2)
#define Z_SHIELD_DAMAGE_SHOT (Z_SHIELD_MAX / 4)
#define Z_SHIELD_DAMAGE_SHOOTING (Z_SHIELD_MAX / 16)
#define Z_SHIELD_BOOST_HEART (Z_SHIELD_MAX / 2)
#define Z_SHIELD_BOOST_REGEN (Z_SHIELD_MAX / (10 * Z_FPS))

#define Z_SHOOT_EVERY_N_FRAMES (Z_FPS / 4)

#define Z_SPEED_MAX (Z_FIX_ONE)
#define Z_SPEED_ACCEL (Z_FIX_ONE / 8)
#define Z_SPEED_DECEL (Z_FIX_ONE / 16)

ZPlayer z_player;
static uint8_t g_heartsBlink = 0;

static bool useShield(int16_t Damage)
{
    bool protected = z_player.shield >= Damage;

    z_player.shield = z_int16_max(i16(z_player.shield - Damage), 0);

    #if Z_DEBUG_INVINCIBLE
        return true;
    #endif

    return protected;
}

static void boostShield(int16_t Boost)
{
    z_player.shield = z_int16_min(i16(z_player.shield + Boost), Z_SHIELD_MAX);
}

void z_player_init(int8_t X, int8_t Y)
{
    z_player.x = z_fix_itofix(X);
    z_player.y = z_fix_itofix(Y);
    z_player.w = 10;
    z_player.h = 4;
    z_player.dx = 0;
    z_player.dy = 0;
    z_player.lastShot = z_fps_getCounter();
    z_player.frame = 0;
    z_player.shootShift = 0;
    z_player.jetFlicker = false;
    z_player.shield = Z_SHIELD_MAX;
    z_player.health = Z_HEALTH_MAX;
}

void z_player_tick(void)
{
    ZFix maxSpeed = Z_SPEED_MAX;

    if(z_button_pressed(z_controls.a)) {
        maxSpeed = Z_SPEED_MAX * 2 / 3;

        if(z_fps_isNthFrame(Z_FPS / 10)) {
            z_player.shootShift ^= 1;
        }

        if(z_fps_getCounter() - z_player.lastShot >= Z_SHOOT_EVERY_N_FRAMES) {
            ZBulletP* b = z_pool_alloc(Z_POOL_BULLETP);

            if(b) {
                z_bulletp_init(b,
                               zf(z_player.x + z_fix_itofix(z_screen_xShake)),
                               z_player.y);
            }

            z_player.lastShot = z_fps_getCounter();
            useShield(Z_SHIELD_DAMAGE_SHOOTING);
        }
    } else {
        z_player.shootShift = 0;
        z_player.lastShot = u16(z_fps_getCounter() - Z_SHOOT_EVERY_N_FRAMES);
    }

    z_player.frame = Z_BIT_RESTING;

    if(z_button_pressed(z_controls.up)) {
        z_player.frame |= Z_BIT_FORWARD;
        z_player.dy = zf(z_player.dy - Z_SPEED_ACCEL);
    } else if(z_button_pressed(z_controls.down)) {
        z_player.frame |= Z_BIT_BACK;
        z_player.dy = zf(z_player.dy + Z_SPEED_ACCEL);
    } else {
        if(z_player.dy < 0) {
            z_player.dy = z_fix_min(zf(z_player.dy + Z_SPEED_DECEL), 0);
        } else if(z_player.dy > 0) {
            z_player.dy = z_fix_max(zf(z_player.dy - Z_SPEED_DECEL), 0);
        }
    }

    if(z_button_pressed(z_controls.left)) {
        z_player.frame |= Z_BIT_LEFT;
        z_player.dx = zf(z_player.dx - Z_SPEED_ACCEL);
    } else if(z_button_pressed(z_controls.right)) {
        z_player.frame |= Z_BIT_RIGHT;
        z_player.dx = zf(z_player.dx + Z_SPEED_ACCEL);
    } else {
        if(z_player.dx < 0) {
            z_player.dx = z_fix_min(zf(z_player.dx + Z_SPEED_DECEL), 0);
        } else if(z_player.dx > 0) {
            z_player.dx = z_fix_max(zf(z_player.dx - Z_SPEED_DECEL), 0);
        }
    }

    z_player.dx = z_fix_clamp(z_player.dx, zf(-maxSpeed), maxSpeed);
    z_player.dy = z_fix_clamp(z_player.dy, zf(-maxSpeed), maxSpeed);

    z_player.x = z_fix_clamp(zf(z_player.x + z_player.dx),
                             0,
                             z_fix_itofix(Z_WIDTH - 1));

    z_player.y = z_fix_clamp(zf(z_player.y + z_player.dy),
                             0,
                             z_fix_itofix(Z_HEIGHT - 1));

    z_player.jetFlicker = !z_player.jetFlicker;

    bool hit = z_enemy_checkCollisions(z_fix_fixtoi(z_player.x),
                                       z_fix_fixtoi(z_player.y),
                                       z_player.w,
                                       z_player.h,
                                       true);

    if(hit) {
        z_player_takeDamage(Z_SHIELD_DAMAGE_COLLISION);
    } else {
        boostShield(Z_SHIELD_BOOST_REGEN);
    }

    if(z_player.health <= 0 && z_fps_isNthFrame(10)) {
        g_heartsBlink ^= 1;
    }
}

static void drawHearts(int8_t X, int8_t Y)
{
    for(int8_t i = 0; i < Z_HEALTH_MAX; i++) {
        uint8_t heartFrame = z_player.health > 0
                             ? z_player.health > i
                             : g_heartsBlink;

        z_sprite_blit(&z_graphics.hearts, i8(X + i * 8), Y, heartFrame);
    }
}

static void drawShield(int8_t X, int8_t Y)
{
    z_sprite_blit(&z_graphics.shield, X, Y, 0);

    int8_t rX = i8(X + 7);
    int8_t rY = i8(Y + 2);
    int8_t maxWidth = 21;
    int8_t width = i8(maxWidth * z_player.shield / Z_SHIELD_MAX);
    int8_t height = 1;

    z_draw_rectangle(i8(rX - 1),
                     i8(rY - 1),
                     i8(maxWidth + 4),
                     i8(height + 4),
                     Z_COLOR_BLUE);
    z_draw_rectangle(rX,
                     rY,
                     i8(maxWidth + 2),
                     i8(height + 2),
                     Z_COLOR_RED);
    z_draw_rectangle(i8(rX + 1 + width),
                     i8(rY + 1),
                     i8(maxWidth - width),
                     height,
                     Z_COLOR_BLUE);
}

void z_player_draw(void)
{
    int8_t x = z_fix_fixtoi(z_player.x);
    int8_t y = i8(z_fix_fixtoi(z_player.y) + z_player.shootShift);

    ZSprite* sprite = &z_graphics.player[z_player.frame];

    if(z_player.jetFlicker) {
        int8_t jy = i8(y + 2 + z_screen_yShake);

        if(z_player.frame & Z_BIT_BACK) {
            jy = i8(y - 1 + z_screen_yShake);
        }

        z_draw_rectangle(i8(x - 3), jy, 2, 3, Z_COLOR_RED);
        z_draw_rectangle(i8(x + 1), jy, 2, 3, Z_COLOR_RED);
    }

    z_sprite_blitCentered(sprite,
                          i8(x + z_screen_xShake),
                          i8(y + z_screen_yShake),
                          0);

    drawHearts(2, 2);
    drawShield(28, 2);
}

void z_player_takeDamage(int16_t Damage)
{
    if(!useShield(Damage)) {
        if(--z_player.health >= 0) {
            boostShield(Z_SHIELD_BOOST_HEART);
        }
    }
}

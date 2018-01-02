/*
    Copyright 2017, 2018 Alex Margarit <alex@alxm.org>

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
#include "util_collision.h"
#include "util_fix.h"
#include "util_fps.h"
#include "util_graphics.h"
#include "util_input.h"
#include "util_pool.h"
#include "util_random.h"
#include "util_screen.h"
#include "obj_bulletp.h"
#include "obj_circle.h"
#include "obj_enemy.h"
#include "obj_particle.h"
#include "obj_player.h"

#define Z_SHOOT_EVERY_DS 3

#define Z_SPEED_SCALE_DIV 2
#define Z_SPEED_MAX (Z_FIX_ONE / Z_SPEED_SCALE_DIV)
#define Z_SPEED_ACCEL (Z_FIX_ONE / 8 / Z_SPEED_SCALE_DIV)
#define Z_SPEED_DECEL (Z_FIX_ONE / 16 / Z_SPEED_SCALE_DIV)

#define Z_SHIELD_DAMAGE_COLLISION Z_SHIELD_MAX
#define Z_SHIELD_DAMAGE_SHOOTING 3
#define Z_SHIELD_REGEN_EVERY_DS 20

#define Z_ENERGY_USE_SHOOTING 2
#define Z_ENERGY_REGEN_EVERY_DS 10

#define Z_INVINCIBLE_TIMER_DS 20

ZPlayer z_player;

static bool hasEnergy(uint8_t Amount)
{
    #if Z_DEBUG_INFINITE_ENERGY
        return true;
    #else
        return z_player.energy >= Amount;
    #endif
}

static bool useEnergy(uint8_t Amount)
{
    bool enough = z_player.energy >= Amount;

    if(enough) {
        z_player.energy = u4(z_player.energy - Amount);
    } else {
        z_player.energy = 0;
    }

    #if Z_DEBUG_INFINITE_ENERGY
        return true;
    #else
        return enough;
    #endif
}

static void boostEnergy(uint8_t Boost)
{
    if(Z_ENERGY_MAX - z_player.energy > Boost) {
        z_player.energy = u4(z_player.energy + Boost);
    } else {
        z_player.energy = Z_ENERGY_MAX;
    }
}

static bool useShield(uint8_t Damage)
{
    bool protected = z_player.shield >= Damage;

    if(protected) {
        z_player.shield = u4(z_player.shield - Damage);
    } else {
        z_player.shield = 0;
    }

    #if Z_DEBUG_INVINCIBLE
        return true;
    #else
        return protected;
    #endif
}

static void boostShield(uint8_t Boost)
{
    if(Z_SHIELD_MAX - z_player.shield > Boost) {
        z_player.shield = u4(z_player.shield + Boost);
    } else {
        z_player.shield = Z_SHIELD_MAX;
    }
}

void z_player_init(int16_t X, int16_t Y)
{
    z_player.x = z_fix_itofix(X);
    z_player.y = z_fix_itofix(Y);
    z_player.dx = 0;
    z_player.dy = 0;
    z_player.w = 10;
    z_player.h = 4;
    z_player.frame = 0;
    z_player.energy = Z_ENERGY_MAX;
    z_player.shield = Z_SHIELD_MAX;
    z_player.health = Z_HEALTH_MAX;
    z_player.lastShotCounter = 0;
    z_player.shootShift = 0;
    z_player.jetFlicker = false;
    z_player.damage = 1;
    z_player.invincibleTimerDs = 0;
}

void z_player_tick(void)
{
    int8_t maxSpeed = Z_SPEED_MAX;

    if(z_button_pressed(Z_BUTTON_A) && hasEnergy(Z_ENERGY_USE_SHOOTING)) {
        maxSpeed = Z_SPEED_MAX / 2;

        Z_EVERY_DS(1) {
            z_player.shootShift ^= 1;
        }

        if(z_player.lastShotCounter-- == 0) {
            ZBulletP* b = z_pool_alloc(Z_POOL_BULLETP);

            if(b) {
                z_bulletp_init(b,
                               zf(z_player.x
                                    + z_fix_itofix(z_screen_getXShake())),
                               z_player.y);

                z_player.lastShotCounter = Z_DS_TO_FRAMES(Z_SHOOT_EVERY_DS);
                useEnergy(Z_ENERGY_USE_SHOOTING);
            } else {
                z_player.lastShotCounter = 0;
            }
        }
    } else {
        z_player.shootShift = 0;
        z_player.lastShotCounter = 0;

        Z_EVERY_DS(Z_ENERGY_REGEN_EVERY_DS) {
            boostEnergy(1);
        }
    }

    z_player.frame = Z_BIT_RESTING;

    if(z_button_pressed(Z_BUTTON_UP)) {
        z_player.frame |= Z_BIT_FORWARD;
        z_player.dy = i8(z_player.dy - Z_SPEED_ACCEL);
    } else if(z_button_pressed(Z_BUTTON_DOWN)) {
        z_player.frame |= Z_BIT_BACK;
        z_player.dy = i8(z_player.dy + Z_SPEED_ACCEL);
    } else {
        if(z_player.dy < 0) {
            z_player.dy = z_int8_min(i8(z_player.dy + Z_SPEED_DECEL), 0);
        } else if(z_player.dy > 0) {
            z_player.dy = z_int8_max(i8(z_player.dy - Z_SPEED_DECEL), 0);
        }
    }

    if(z_button_pressed(Z_BUTTON_LEFT)) {
        z_player.frame |= Z_BIT_LEFT;
        z_player.dx = i8(z_player.dx - Z_SPEED_ACCEL);
    } else if(z_button_pressed(Z_BUTTON_RIGHT)) {
        z_player.frame |= Z_BIT_RIGHT;
        z_player.dx = i8(z_player.dx + Z_SPEED_ACCEL);
    } else {
        if(z_player.dx < 0) {
            z_player.dx = z_int8_min(i8(z_player.dx + Z_SPEED_DECEL), 0);
        } else if(z_player.dx > 0) {
            z_player.dx = z_int8_max(i8(z_player.dx - Z_SPEED_DECEL), 0);
        }
    }

    z_player.dx = z_int8_clamp(z_player.dx, i8(-maxSpeed), maxSpeed);
    z_player.dy = z_int8_clamp(z_player.dy, i8(-maxSpeed), maxSpeed);

    z_player.x = z_fix_clamp(zf(z_player.x + z_player.dx * Z_SPEED_SCALE_DIV),
                             0,
                             z_fix_itofix(Z_WIDTH - 1));

    z_player.y = z_fix_clamp(zf(z_player.y + z_player.dy * Z_SPEED_SCALE_DIV),
                             0,
                             z_fix_itofix(Z_HEIGHT - 1));

    z_player.jetFlicker = !z_player.jetFlicker;

    bool hit = z_collision_checkEnemyShips(z_fix_fixtoi(z_player.x),
                                           z_fix_fixtoi(z_player.y),
                                           z_player.w,
                                           z_player.h,
                                           Z_ENEMY_MAX_HEALTH,
                                           true);

    if(hit) {
        z_player_takeDamage(Z_SHIELD_DAMAGE_COLLISION);
    } else Z_EVERY_DS(Z_SHIELD_REGEN_EVERY_DS) {
        boostShield(1);
    }

    if(z_player.invincibleTimerDs > 0) {
        Z_EVERY_DS(1) {
            z_player.invincibleTimerDs--;
        }
    }
}

void z_player_draw(void)
{
    int16_t x = z_fix_fixtoi(z_player.x);
    int16_t y = i16(z_fix_fixtoi(z_player.y) + z_player.shootShift);

    if(z_player.jetFlicker) {
        int16_t jy = i16(y + 2 + z_screen_getYShake());

        if(z_player.frame & Z_BIT_BACK) {
            jy = i16(y - 1 + z_screen_getYShake());
        }

        z_draw_rectangle(i16(x - 3), jy, 2, 3, Z_COLOR_RED);
        z_draw_rectangle(i16(x + 1), jy, 2, 3, Z_COLOR_RED);
    }

    int16_t fx = i16(1
                     - !!(z_player.frame & Z_BIT_LEFT)
                     + !!(z_player.frame & Z_BIT_RIGHT));
    int16_t fy = i16(1
                     - !!(z_player.frame & Z_BIT_FORWARD)
                     + !!(z_player.frame & Z_BIT_BACK));

    z_sprite_blitCentered(Z_SPRITE_PLAYER,
                          i16(x + z_screen_getXShake()),
                          i16(y + z_screen_getYShake()),
                          u8(fy * 3 + fx));

    if(z_player.invincibleTimerDs > 0) {
        if(z_player.jetFlicker) {
            z_draw_circle(x, z_fix_fixtoi(z_player.y), 9, Z_COLOR_RED);
        } else {
            z_draw_circle(x, z_fix_fixtoi(z_player.y), 9, Z_COLOR_LIGHTBLUE2);
        }
    }
}

void z_player_takeDamage(uint8_t Damage)
{
    if(z_player.health < 0 || z_player.invincibleTimerDs > 0) {
        return;
    }

    if(!useShield(Damage)) {
        if(--z_player.health >= 0) {
            boostShield(Z_SHIELD_MAX);
            z_player.invincibleTimerDs = Z_INVINCIBLE_TIMER_DS;
        }
    }
}

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
#include "obj_player.h"

#include "obj_bulletp.h"
#include "util_fps.h"
#include "util_effects.h"
#include "util_input.h"
#include "util_screen.h"
#include "util_timer.h"

#define Z_PLAYER_SHOOT_EVERY_DS        (3)
#define Z_PLAYER_SHOOT_SHIFT_DS        (1)

#define Z_PLAYER_SPEED_MAX             (Z_FIX_ONE * 3 / 4)
#define Z_PLAYER_SPEED_ACCEL           (Z_FIX_ONE / 8)
#define Z_PLAYER_SPEED_DECEL           (Z_FIX_ONE / 16)

#define Z_PLAYER_SHIELD_REGEN_EVERY_DS (20)

#define Z_PLAYER_ENERGY_USE_SHOOTING   (1)
#define Z_PLAYER_ENERGY_REGEN_EVERY_DS (10)

#define Z_PLAYER_INVINCIBLE_TIMER_DS   (20)

typedef struct ZPlayer {
    ZFix x, y;
    int16_t dx, dy;
    uint8_t frame : 4;
    uint8_t energy : 4;
    uint8_t shield : 4;
    int8_t health : 4;
    uint8_t lastShotCounter : 5;
    uint8_t shootShift : 1;
    bool jetFlicker : 1;
    bool invincible : 1;
    uint8_t damage : 3;
    uint16_t score;
    uint8_t level;
} ZPlayer;

static ZPlayer g_player;

static bool hasEnergy(uint8_t Amount)
{
    #if Z_DEBUG_INFINITE_ENERGY
        Z_UNUSED(Amount);
        return true;
    #else
        return g_player.energy >= Amount;
    #endif
}

static bool useEnergy(uint8_t Amount)
{
    bool enough = g_player.energy >= Amount;

    if(enough) {
        g_player.energy = u4(g_player.energy - Amount);
    } else {
        g_player.energy = 0;
    }

    #if Z_DEBUG_INFINITE_ENERGY
        return true;
    #else
        return enough;
    #endif
}

static void boostEnergy(uint8_t Boost)
{
    if(Z_PLAYER_MAX_ENERGY - g_player.energy > Boost) {
        g_player.energy = u4(g_player.energy + Boost);
    } else {
        g_player.energy = Z_PLAYER_MAX_ENERGY;
    }
}

static bool useShield(uint8_t Damage)
{
    bool protected = g_player.shield >= Damage;

    if(protected) {
        g_player.shield = u4(g_player.shield - Damage);
    } else {
        g_player.shield = 0;
    }

    #if Z_DEBUG_INVINCIBLE
        return true;
    #else
        return protected;
    #endif
}

static void boostShield(uint8_t Boost)
{
    if(Z_PLAYER_MAX_SHIELD - g_player.shield > Boost) {
        g_player.shield = u4(g_player.shield + Boost);
    } else {
        g_player.shield = Z_PLAYER_MAX_SHIELD;
    }
}

void z_player_init(void)
{
    g_player.frame = 0;
    g_player.energy = Z_PLAYER_MAX_ENERGY;
    g_player.shield = Z_PLAYER_MAX_SHIELD;
    g_player.health = Z_PLAYER_MAX_HEALTH;
    g_player.lastShotCounter = 0;
    g_player.shootShift = 0;
    g_player.jetFlicker = false;
    g_player.damage = 1;
    g_player.invincible = false;
    g_player.score = 0;
    g_player.level = 1;

    z_player_resetPosition();

    z_timer_start(Z_TIMER_PLAYER_REGEN_ENERGY, Z_PLAYER_ENERGY_REGEN_EVERY_DS);
    z_timer_start(Z_TIMER_PLAYER_REGEN_SHIELD, Z_PLAYER_SHIELD_REGEN_EVERY_DS);
}

void z_player_tick(bool CheckInput)
{
    if(g_player.health < 0) {
        return;
    }

    int16_t maxSpeed = Z_PLAYER_SPEED_MAX;

    if(CheckInput && z_button_pressed(Z_BUTTON_A)
        && hasEnergy(Z_PLAYER_ENERGY_USE_SHOOTING)) {

        maxSpeed = Z_PLAYER_SPEED_MAX / 2;

        if(g_player.lastShotCounter-- == 0) {
            ZBulletP* b = z_pool_alloc(Z_POOL_BULLETP);

            if(b) {
                z_bulletp_init(b,
                               zf(g_player.x
                                    + z_fix_fromInt(z_screen_getXShake())),
                               g_player.y);

                g_player.shootShift = 1;
                g_player.lastShotCounter = u5(
                    z_timer_dsToTicks(Z_PLAYER_SHOOT_EVERY_DS));

                useEnergy(Z_PLAYER_ENERGY_USE_SHOOTING);
                z_timer_start(Z_TIMER_PLAYER_SHOOT, Z_PLAYER_SHOOT_SHIFT_DS);

                z_sfx_play(Z_SFX_PLAYER_SHOOT);
            } else {
                g_player.lastShotCounter = 0;
            }
        }
    } else {
        g_player.shootShift = 0;
        g_player.lastShotCounter = 0;

        if(z_timer_expired(Z_TIMER_PLAYER_REGEN_ENERGY)) {
            boostEnergy(1);
        }
    }

    if(g_player.shootShift && z_timer_expired(Z_TIMER_PLAYER_SHOOT)) {
        g_player.shootShift = 0;
    }

    g_player.frame = Z_BIT_RESTING;

    if(CheckInput && z_button_pressed(Z_BUTTON_UP)) {
        g_player.frame |= Z_BIT_FORWARD;
        g_player.dy = i16(g_player.dy - Z_PLAYER_SPEED_ACCEL);
    } else if(CheckInput && z_button_pressed(Z_BUTTON_DOWN)) {
        g_player.frame |= Z_BIT_BACK;
        g_player.dy = i16(g_player.dy + Z_PLAYER_SPEED_ACCEL);
    } else {
        if(g_player.dy < 0) {
            g_player.dy = z_min_int16(i16(g_player.dy + Z_PLAYER_SPEED_DECEL),
                                      0);
        } else if(g_player.dy > 0) {
            g_player.dy = z_max_int16(i16(g_player.dy - Z_PLAYER_SPEED_DECEL),
                                      0);
        }
    }

    if(CheckInput && z_button_pressed(Z_BUTTON_LEFT)) {
        g_player.frame |= Z_BIT_LEFT;
        g_player.dx = i16(g_player.dx - Z_PLAYER_SPEED_ACCEL);
    } else if(CheckInput && z_button_pressed(Z_BUTTON_RIGHT)) {
        g_player.frame |= Z_BIT_RIGHT;
        g_player.dx = i16(g_player.dx + Z_PLAYER_SPEED_ACCEL);
    } else {
        if(g_player.dx < 0) {
            g_player.dx = z_min_int16(i16(g_player.dx + Z_PLAYER_SPEED_DECEL),
                                      0);
        } else if(g_player.dx > 0) {
            g_player.dx = z_max_int16(i16(g_player.dx - Z_PLAYER_SPEED_DECEL),
                                      0);
        }
    }

    g_player.dx = z_clamp_int16(g_player.dx, i16(-maxSpeed), maxSpeed);
    g_player.dy = z_clamp_int16(g_player.dy, i16(-maxSpeed), maxSpeed);

    g_player.x = z_clamp_fix(zf(g_player.x + g_player.dx),
                             0,
                             z_fix_fromInt(Z_SCREEN_W - 1));

    g_player.y = z_clamp_fix(zf(g_player.y + g_player.dy),
                             0,
                             z_fix_fromInt(Z_SCREEN_H - 1));

    g_player.jetFlicker = !g_player.jetFlicker;

    if(z_timer_expired(Z_TIMER_PLAYER_REGEN_SHIELD)) {
        boostShield(1);
    }

    if(g_player.invincible && z_timer_expired(Z_TIMER_PLAYER_INVINCIBLE)) {
        g_player.invincible = false;
    }
}

void z_player_draw(void)
{
    if(g_player.health < 0) {
        return;
    }

    int16_t x = z_fix_toInt(g_player.x);
    int16_t y = i16(z_fix_toInt(g_player.y) + g_player.shootShift);

    if(g_player.jetFlicker) {
        int16_t jy = i16(y + 2 + z_screen_getYShake());

        if(g_player.frame & Z_BIT_BACK) {
            jy = i16(y - 1 + z_screen_getYShake());
        }

        z_draw_rectangle(i16(x - 3), jy, 2, 3, Z_COLOR_RED);
        z_draw_rectangle(i16(x + 1), jy, 2, 3, Z_COLOR_RED);
    }

    int16_t fx = i16(1
                     - !!(g_player.frame & Z_BIT_LEFT)
                     + !!(g_player.frame & Z_BIT_RIGHT));
    int16_t fy = i16(1
                     - !!(g_player.frame & Z_BIT_FORWARD)
                     + !!(g_player.frame & Z_BIT_BACK));

    z_sprite_blitCentered(Z_SPRITE_PLAYER,
                          i16(x + z_screen_getXShake()),
                          i16(y + z_screen_getYShake()),
                          u8(fy * 3 + fx));

    if(g_player.invincible) {
        if(g_player.jetFlicker) {
            z_draw_circle(x, z_fix_toInt(g_player.y), 9, Z_COLOR_RED);
        } else {
            z_draw_circle(x, z_fix_toInt(g_player.y), 9, Z_COLOR_GRAY);
        }
    }

    if(g_player.shootShift) {
        z_effect_light(g_player.x, g_player.y, Z_LIGHT_PLAYER_SHOOTING);
    }
}

void z_player_resetPosition(void)
{
    g_player.dx = 0;
    g_player.dy = 0;
    g_player.x = z_fix_fromInt(Z_SCREEN_W / 2);
    g_player.y = z_fix_fromInt(Z_SCREEN_H / 2);
}

void z_player_takeDamage(uint8_t Damage)
{
    if(g_player.health < 0 || g_player.invincible) {
        return;
    }

    if(useShield(Damage)) {
        z_sfx_play(Z_SFX_PLAYER_HURT);
    } else if(g_player.health-- == 0) {
        z_sfx_play(Z_SFX_PLAYER_DIE);
    } else {
        z_sfx_play(Z_SFX_SHIELD_DEPLOY);
        boostShield(Z_PLAYER_MAX_SHIELD);
        z_timer_start(Z_TIMER_PLAYER_INVINCIBLE, Z_PLAYER_INVINCIBLE_TIMER_DS);
        g_player.invincible = true;
    }
}

void z_player_scorePoints(uint8_t Points)
{
    g_player.score = u16(g_player.score + Points);
}

uint8_t z_player_getLevel(void)
{
    return g_player.level;
}

void z_player_setLevel(uint8_t Level)
{
    g_player.level = Level;
}

uint16_t z_player_getScore(void)
{
    return g_player.score;
}

uint8_t z_player_getEnergy(void)
{
    return g_player.energy;
}

uint8_t z_player_getShield(void)
{
    return g_player.shield;
}

int8_t z_player_getHealth(void)
{
    return g_player.health;
}

void z_player_getCoords(ZFix* X, ZFix* Y)
{
    *X = g_player.x;
    *Y = g_player.y;
}

bool z_player_getInvincible(void)
{
    return g_player.invincible;
}

uint8_t z_player_getDamage(void)
{
    return g_player.damage;
}

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
#include "util_hud.h"

#include "obj_player.h"
#include "util_font.h"
#include "util_timer.h"

#define Z_HUD_BLINK_INTERVAL_DS 3

static struct {
    uint16_t score;
    bool fillHearts;
    bool fillEnergy;
} g_hud;

void z_hud_reset(void)
{
    g_hud.score = 0;
    g_hud.fillHearts = false;
    g_hud.fillEnergy = false;

    z_timer_stop(Z_TIMER_HUD_HEARTS);
    z_timer_start(Z_TIMER_HUD_SCORE, 1);
}

void z_hud_tick(void)
{
    if(z_player_getHealth() <= 0) {
        if(!z_timer_running(Z_TIMER_HUD_HEARTS)) {
            z_timer_start(Z_TIMER_HUD_HEARTS, Z_HUD_BLINK_INTERVAL_DS);
            g_hud.fillHearts = true;
        } else if(z_timer_expired(Z_TIMER_HUD_HEARTS)) {
            g_hud.fillHearts = !g_hud.fillHearts;
        }
    } else {
        z_timer_stop(Z_TIMER_HUD_HEARTS);
        g_hud.fillHearts = false;
    }

    if(z_player_getEnergy() < 1) {
        if(!z_timer_running(Z_TIMER_HUD_ENERGY)) {
            z_timer_start(Z_TIMER_HUD_ENERGY, Z_HUD_BLINK_INTERVAL_DS);
            g_hud.fillEnergy = true;
        } else if(z_timer_expired(Z_TIMER_HUD_ENERGY)) {
            g_hud.fillEnergy = !g_hud.fillEnergy;
        }
    } else {
        z_timer_stop(Z_TIMER_HUD_ENERGY);
        g_hud.fillEnergy = false;
    }

    if(z_timer_expired(Z_TIMER_HUD_SCORE)) {
        uint16_t score = z_player_getScore();

        if(g_hud.score < score) {
            g_hud.score = u16(g_hud.score + (score - g_hud.score) / 2 + 1);
        }
    }
}

static void drawHearts(int16_t X, int16_t Y)
{
    for(int8_t i = 0; i < Z_PLAYER_MAX_HEALTH; i++) {
        int8_t health = z_player_getHealth();
        uint8_t heartFrame = g_hud.fillHearts || health > i;

        z_sprite_blit(Z_SPRITE_HEARTS, i16(X + i * 8), Y, heartFrame);
    }
}

static void drawBar(int16_t X, int16_t Y, uint8_t Value, uint8_t Max)
{
    int8_t height = 1;
    int8_t maxWidth = 15;
    int8_t width = i8(maxWidth * Value / Max);

    z_draw_rectangle(X,
                     Y,
                     i8(maxWidth + 4),
                     i8(height + 4),
                     Z_COLOR_BLUE);

    z_draw_rectangle(i16(X + 1),
                     i16(Y + 1),
                     i8(maxWidth + 2),
                     i8(height + 2),
                     Z_COLOR_RED);

    z_draw_rectangle(i16(X + 2 + width),
                     i16(Y + 2),
                     i8(maxWidth - width),
                     height,
                     Z_COLOR_BLUE);
}

static void drawShield(int16_t X, int16_t Y)
{
    z_sprite_blit(Z_SPRITE_SHIELD, X, Y, 0);
    drawBar(i16(X + 6), i16(Y + 1), z_player_getShield(), Z_PLAYER_MAX_SHIELD);
}

static void drawEnergy(int16_t X, int16_t Y)
{
    z_sprite_blit(Z_SPRITE_ENERGY, X, Y, 0);

    uint8_t value = g_hud.fillEnergy
                    ? Z_PLAYER_MAX_ENERGY
                    : z_player_getEnergy();

    drawBar(i16(X + 4), i16(Y + 2), value, Z_PLAYER_MAX_ENERGY);
}

static void drawLevel(int16_t X, int16_t Y)
{
    int16_t x = i16(X - z_sprite_getWidth(Z_SPRITE_ICON_LEVEL));

    z_sprite_blit(Z_SPRITE_ICON_LEVEL, x, i16(Y + 1), 0);

    z_font_int(z_player_getLevel(),
               i16(x - 1),
               Y,
               Z_FONT_FACE_LCD,
               Z_FONT_ALIGN_R);
}

static void drawScore(int16_t X, int16_t Y)
{
    int16_t x = i16(X - z_sprite_getWidth(Z_SPRITE_ICON_CREDS));

    z_sprite_blit(Z_SPRITE_ICON_CREDS, x, i16(Y + 1), 0);

    z_font_int(i16(g_hud.score),
               i16(x - 1),
               Y,
               Z_FONT_FACE_LCD,
               Z_FONT_ALIGN_R);
}

void z_hud_draw(void)
{
    drawHearts(2, 2);
    drawShield(28, 2);
    drawEnergy(54, 1);
    drawLevel(Z_SCREEN_W - 2, Z_SCREEN_H - 17);
    drawScore(Z_SCREEN_W - 2, Z_SCREEN_H - 9);
}

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
#include "util_fix.h"
#include "util_font.h"
#include "util_graphics.h"
#include "util_screen.h"
#include "util_timer.h"
#include "obj_player.h"

void z_hud_reset(void)
{
    z_timer_start(Z_TIMER_HUD_HEARTS, 3);
    z_timer_start(Z_TIMER_HUD_SCORE, 1);
}

void z_hud_tick(void)
{
    if(z_player.health <= 0) {
        if(z_timer_expired(Z_TIMER_HUD_HEARTS)) {
            z_player.heartsBlink ^= 1;
        }
    }

    if(z_timer_expired(Z_TIMER_HUD_SCORE)) {
        if(z_player.scoreShow < z_player.score) {
            z_player.scoreShow =
                u16(z_player.scoreShow
                    + (z_player.score - z_player.scoreShow) / 2 + 1);
        }
    }
}

static void drawHearts(int16_t X, int16_t Y)
{
    for(int8_t i = 0; i < Z_HEALTH_MAX; i++) {
        uint8_t heartFrame = z_player.health > 0
                             ? z_player.health > i
                             : z_player.heartsBlink;

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
    drawBar(i16(X + 6), i16(Y + 1), z_player.shield, Z_SHIELD_MAX);
}

static void drawEnergy(int16_t X, int16_t Y)
{
    z_sprite_blit(Z_SPRITE_ENERGY, X, Y, 0);
    drawBar(i16(X + 4), i16(Y + 2), z_player.energy, Z_ENERGY_MAX);
}

static void drawScore(int16_t X, int16_t Y)
{
    int16_t credsX = i16(X - z_sprite_getWidth(Z_SPRITE_CREDS));

    z_sprite_blit(Z_SPRITE_CREDS, credsX, i16(Y + 1), 0);

    z_font_int(i16(z_player.scoreShow),
               i16(credsX - 1),
               Y,
               Z_FONT_FACE_LCD,
               Z_FONT_ALIGN_R);
}

void z_hud_draw(void)
{
    drawHearts(2, 2);
    drawShield(28, 2);
    drawEnergy(54, 1);
    drawScore(i16(Z_SCREEN_W - 2), i16(Z_SCREEN_H - 9));
}

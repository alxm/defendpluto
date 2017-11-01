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

#include <Arduino.h>
#include <Arduboy2.h>
#include <Sprites.h>
#include "shared.h"

#include "data_gfx_enemy00.h"
#include "data_gfx_enemy01.h"
#include "data_gfx_enemy02.h"
#include "data_gfx_player.h"

extern Arduboy2Base g_arduboy;
ZControls z_controls;
ZGfx z_gfx;

void z_shared_setup(void)
{
    z_controls.up = UP_BUTTON;
    z_controls.down = DOWN_BUTTON;
    z_controls.left = LEFT_BUTTON;
    z_controls.right = RIGHT_BUTTON;
    z_controls.a = A_BUTTON;
    z_controls.b = B_BUTTON;

    z_gfx.enemy[0] = z_data_gfx_enemy00_buffer;
    z_gfx.enemy[1] = z_data_gfx_enemy01_buffer;
    z_gfx.enemy[2] = z_data_gfx_enemy02_buffer;
    z_gfx.playerShip = z_data_gfx_player_buffer;
}

uint16_t z_fps_getCounter(void)
{
    return g_arduboy.frameCount;
}

bool z_fps_isNthFrame(uint8_t N)
{
    return g_arduboy.everyXFrames(N);
}

bool z_button_pressed(ZButton Button)
{
    return g_arduboy.pressed(Button);
}

void z_draw_fill(bool White)
{
    g_arduboy.fillScreen(White ? WHITE : BLACK);
}

void z_draw_rectangle(int8_t X, int8_t Y, int8_t W, int8_t H, bool White)
{
    g_arduboy.fillRect(X, Y, W, H, White ? WHITE : BLACK);
}

void z_draw_pixel(int8_t X, int8_t Y, bool White)
{
    g_arduboy.drawPixel(X, Y, White ? WHITE : BLACK);
}

void z_sprite_blit(ZSprite Sprite, int8_t X, int8_t Y)
{
    Sprites::drawSelfMasked(X, Y, Sprite, 0);
}

int8_t z_sprite_getWidth(ZSprite Sprite)
{
    return (int8_t)pgm_read_byte(Sprite);
}

int8_t z_sprite_getHeight(ZSprite Sprite)
{
    return (int8_t)pgm_read_byte(Sprite + 1);
}

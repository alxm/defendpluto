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
#include <Sprites.h>
#include "data_gfx_enemy00.h"
#include "data_gfx_enemy01.h"
#include "data_gfx_enemy02.h"
#include "data_gfx_player.h"
#include "data_gfx_player_left.h"
#include "data_gfx_player_right.h"
#include "data_gfx_player_forward.h"
#include "data_gfx_player_forward_left.h"
#include "data_gfx_player_forward_right.h"
#include "data_gfx_player_back.h"
#include "data_gfx_player_back_left.h"
#include "data_gfx_player_back_right.h"

extern Arduboy2Base g_arduboy;
ZControls z_controls;
ZGfx z_gfx;

void z_platform_setup(void)
{
    z_controls.up = UP_BUTTON;
    z_controls.down = DOWN_BUTTON;
    z_controls.left = LEFT_BUTTON;
    z_controls.right = RIGHT_BUTTON;
    z_controls.a = A_BUTTON;
    z_controls.b = B_BUTTON;

    #define makeStruct(s) \
        (ZSprite){z_data_gfx_##s##_buffer, z_data_gfx_##s##_mask}

    z_gfx.enemy[0] = makeStruct(enemy00);
    z_gfx.enemy[1] = makeStruct(enemy01);
    z_gfx.enemy[2] = makeStruct(enemy02);
    z_gfx.player[0] = makeStruct(player);
    z_gfx.player[1] = makeStruct(player_left);
    z_gfx.player[2] = makeStruct(player_right);
    z_gfx.player[3] = makeStruct(player_forward);
    z_gfx.player[4] = makeStruct(player_forward_left);
    z_gfx.player[5] = makeStruct(player_forward_right);
    z_gfx.player[6] = makeStruct(player_back);
    z_gfx.player[7] = makeStruct(player_back_left);
    z_gfx.player[8] = makeStruct(player_back_right);
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

void z_draw_fill(uint8_t Color)
{
    g_arduboy.fillScreen(Color ? WHITE : BLACK);
}

void z_draw_rectangle(int8_t X, int8_t Y, int8_t W, int8_t H, uint8_t Color)
{
    g_arduboy.fillRect(X, Y, W, H, Color ? WHITE : BLACK);
}

void z_draw_pixel(int8_t X, int8_t Y, uint8_t Color)
{
    g_arduboy.drawPixel(X, Y, Color ? WHITE : BLACK);
}

void z_draw_circle(int8_t X, int8_t Y, uint8_t Radius, uint8_t Color)
{
    g_arduboy.drawCircle(X, Y, Radius, Color ? WHITE : BLACK);
}

void z_sprite_blit(ZSprite Sprite, int8_t X, int8_t Y)
{
    Sprites::drawExternalMask(X, Y, Sprite.sprite, Sprite.mask, 0, 0);
}

int8_t z_sprite_getWidth(ZSprite Sprite)
{
    return (int8_t)pgm_read_byte(Sprite.sprite);
}

int8_t z_sprite_getHeight(ZSprite Sprite)
{
    return (int8_t)pgm_read_byte(Sprite.sprite + 1);
}

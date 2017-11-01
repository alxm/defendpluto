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

#ifndef ARDUINO

#include "shared.h"

ZControls z_controls;
ZGfx z_gfx;

static APixel g_pal[4];

void z_shared_setup(void)
{
    z_controls.up = a_button_new("key.up gamepad.b.up");
    z_controls.down = a_button_new("key.down gamepad.b.down");
    z_controls.left = a_button_new("key.left gamepad.b.left");
    z_controls.right = a_button_new("key.right gamepad.b.right");
    z_controls.a = a_button_new("key.z gamepad.b.a");
    z_controls.b = a_button_new("key.x gamepad.b.b");

    z_gfx.playerShip = a_sprite_newFromFile("gfx/player.png");
    z_gfx.enemy[0] = a_sprite_newFromFile("gfx/enemy00.png");
    z_gfx.enemy[1] = a_sprite_newFromFile("gfx/enemy01.png");
    z_gfx.enemy[2] = a_sprite_newFromFile("gfx/enemy02.png");

    g_pal[0] = a_pixel_hex(0x23293f);
    g_pal[1] = a_pixel_hex(0xb2d26d);
    g_pal[2] = a_pixel_hex(0xd7531f);
    g_pal[3] = a_pixel_hex(0x5f6c79);
}

uint16_t z_fps_getCounter(void)
{
    return (uint16_t)a_fps_getCounter();
}

bool z_fps_isNthFrame(uint8_t N)
{
    return a_fps_isNthFrame(N);
}

bool z_button_pressed(ZButton Button)
{
    return a_button_getPressed(Button);
}

void z_draw_fill(uint8_t Color)
{
    a_pixel_setPixel(g_pal[Color]);
    a_draw_fill();
}

void z_draw_rectangle(int8_t X, int8_t Y, int8_t W, int8_t H, uint8_t Color)
{
    a_pixel_setPixel(g_pal[Color]);
    a_draw_rectangle(X, Y, W, H);
}

void z_draw_pixel(int8_t X, int8_t Y, uint8_t Color)
{
    a_pixel_setPixel(g_pal[Color]);
    a_draw_pixel(X, Y);
}

void z_sprite_blit(ZSprite Sprite, int8_t X, int8_t Y)
{
    a_pixel_setPixel(g_pal[1]);
    a_pixel_setBlitFillFlat(true);
    a_sprite_blit(Sprite, X, Y);
}

int8_t z_sprite_getWidth(ZSprite Sprite)
{
    return (int8_t)a_sprite_getWidth(Sprite);
}

int8_t z_sprite_getHeight(ZSprite Sprite)
{
    return (int8_t)a_sprite_getHeight(Sprite);
}

#endif // ifndef ARDUINO

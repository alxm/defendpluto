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

#include "platform.h"

ZControls z_controls;
ZGfx z_gfx;

static bool g_color = true;
static APixel g_pal[Z_COLOR_NUM][2];
static APixel g_white;
static AInputButton* g_colorSwitch;

static ASpriteFrames* loadSprite(const char* Path)
{
    ASprite* color = a_sprite_newFromFile(Path);
    ASprite* bw = a_sprite_dup(color);

    for(ZColor c = 0; c < Z_COLOR_NUM; c++) {
        a_sprite_replaceColor(bw, g_pal[c][1], g_pal[c][0]);
    }

    ASpriteFrames* frames = a_spriteframes_newBlank(0);

    a_spriteframes_push(frames, color);
    a_spriteframes_push(frames, bw);

    return frames;
}

void z_platform_setup(void)
{
    z_controls.up = a_button_new("key.up gamepad.b.up");
    z_controls.down = a_button_new("key.down gamepad.b.down");
    z_controls.left = a_button_new("key.left gamepad.b.left");
    z_controls.right = a_button_new("key.right gamepad.b.right");
    z_controls.a = a_button_new("key.z gamepad.b.a");
    z_controls.b = a_button_new("key.x gamepad.b.b");

    ASprite* pal = a_sprite_newFromFile("gfx/palette.png");

    for(ZColor c = 0; c < Z_COLOR_NUM; c++) {
        g_pal[c][0] = a_sprite_getPixel(pal, 1 + c, 0);
        g_pal[c][1] = a_sprite_getPixel(pal, 1 + c, 1);
    }

    a_sprite_free(pal);

    g_white = a_pixel_hex(0xffffff);
    g_colorSwitch = a_button_new("key.c gamepad.b.select");

    z_gfx.enemy[0] = loadSprite("gfx/enemy00.png");
    z_gfx.enemy[1] = loadSprite("gfx/enemy01.png");
    z_gfx.enemy[2] = loadSprite("gfx/enemy02.png");
    z_gfx.player[0] = loadSprite("gfx/player.png");
    z_gfx.player[1] = loadSprite("gfx/player_left.png");
    z_gfx.player[2] = loadSprite("gfx/player_right.png");
    z_gfx.player[3] = loadSprite("gfx/player_forward.png");
    z_gfx.player[4] = loadSprite("gfx/player_forward_left.png");
    z_gfx.player[5] = loadSprite("gfx/player_forward_right.png");
    z_gfx.player[6] = loadSprite("gfx/player_back.png");
    z_gfx.player[7] = loadSprite("gfx/player_back_left.png");
    z_gfx.player[8] = loadSprite("gfx/player_back_right.png");
}

void z_platform_tick(void)
{
    if(a_button_getPressedOnce(g_colorSwitch)) {
        g_color = !g_color;
    }
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
    a_pixel_setPixel(g_pal[Color][g_color]);
    a_draw_fill();
}

void z_draw_rectangle(int8_t X, int8_t Y, int8_t W, int8_t H, uint8_t Color)
{
    a_pixel_setPixel(g_pal[Color][g_color]);
    a_draw_rectangle(X, Y, W, H);
}

void z_draw_pixel(int8_t X, int8_t Y, uint8_t Color)
{
    a_pixel_setPixel(g_pal[Color][g_color]);
    a_draw_pixel(X, Y);
}

void z_draw_circle(int8_t X, int8_t Y, uint8_t Radius, uint8_t Color)
{
    a_pixel_setPixel(g_pal[Color][g_color]);
    a_draw_circle(X, Y, Radius);
}

void z_sprite_blit(ZSprite Sprite, int8_t X, int8_t Y)
{
    a_sprite_blit(a_spriteframes_getIndex(Sprite, g_color), X, Y);
}

int8_t z_sprite_getWidth(ZSprite Sprite)
{
    return (int8_t)a_sprite_getWidth(a_spriteframes_getCurrent(Sprite));
}

int8_t z_sprite_getHeight(ZSprite Sprite)
{
    return (int8_t)a_sprite_getHeight(a_spriteframes_getCurrent(Sprite));
}

#endif // ifndef ARDUINO

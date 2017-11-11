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
#include "util_graphics.h"
#include "util_input.h"

ZControls z_controls;

static ZPalette g_paletteIndex;
static AInputButton* g_paletteSwitch;
static APixel g_palettes[Z_PALETTE_NUM][Z_COLOR_NUM];

void z_platform_setup(void)
{
    z_controls.up = a_button_new("key.up gamepad.b.up");
    z_controls.down = a_button_new("key.down gamepad.b.down");
    z_controls.left = a_button_new("key.left gamepad.b.left");
    z_controls.right = a_button_new("key.right gamepad.b.right");
    z_controls.a = a_button_new("key.z gamepad.b.a");
    z_controls.b = a_button_new("key.x gamepad.b.b");

    g_paletteIndex = 0;
    g_paletteSwitch = a_button_new("key.c gamepad.b.select");

    ASprite* pal = a_sprite_newFromFile("gfx/palette.png");

    for(ZPalette p = 0; p < Z_PALETTE_NUM; p++) {
        for(ZColor c = 0; c < Z_COLOR_NUM; c++) {
            g_palettes[p][c] = a_sprite_getPixel(pal, 1 + c, p);
        }
    }

    a_sprite_free(pal);
}

void z_platform_tick(void)
{
    if(a_button_getPressedOnce(g_paletteSwitch)) {
        g_paletteIndex = (g_paletteIndex + 1) % Z_PALETTE_NUM;
    }
}

void z_platform_draw(void)
{
    //
}

uint16_t z_fps_getCounter(void)
{
    return u16(a_fps_getCounter());
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
    a_pixel_setPixel(g_palettes[g_paletteIndex][Color]);
    a_draw_fill();
}

void z_draw_rectangle(int8_t X, int8_t Y, int8_t W, int8_t H, uint8_t Color)
{
    a_pixel_setPixel(g_palettes[g_paletteIndex][Color]);
    a_draw_rectangle(X, Y, W, H);
}

void z_draw_pixel(int8_t X, int8_t Y, uint8_t Color)
{
    a_pixel_setPixel(g_palettes[g_paletteIndex][Color]);
    a_draw_pixel(X, Y);
}

void z_draw_circle(int8_t X, int8_t Y, uint8_t Radius, uint8_t Color)
{
    a_pixel_setPixel(g_palettes[g_paletteIndex][Color]);
    a_draw_circle(X, Y, Radius);
}

void z_platform__loadSprite(ZSprite* Sprite, const char* Path)
{
    ASprite* sheet = a_sprite_newFromFile(Path);
    ASpriteFrames* frames = a_spriteframes_new(sheet, 0, 0, 0);

    for(ZPalette p = 0; p < Z_PALETTE_NUM; p++) {
        if(p == Z_PALETTE_DEFAULT) {
            Sprite->frames[p] = frames;
            continue;
        }

        Sprite->frames[p] = a_spriteframes_dup(frames, true);
        AList* sprites = a_spriteframes_getSprites(Sprite->frames[p]);

        A_LIST_ITERATE(sprites, ASprite*, s) {
            for(ZColor c = 0; c < Z_COLOR_NUM; c++) {
                a_sprite_replaceColor(s,
                                      g_palettes[Z_PALETTE_DEFAULT][c],
                                      g_palettes[p][c]);
            }
        }
    }

    Sprite->numFrames = u8(a_spriteframes_getNum(frames));

    a_sprite_free(sheet);
}

static ASprite* getCurrentSprite(ZSprite* Sprite, uint8_t Frame)
{
    return a_spriteframes_getIndex(Sprite->frames[g_paletteIndex], Frame);
}

void z_sprite_blit(ZSprite* Sprite, int8_t X, int8_t Y, uint8_t Frame)
{
    a_sprite_blit(getCurrentSprite(Sprite, Frame), X, Y);
}

int8_t z_sprite_getWidth(ZSprite* Sprite)
{
    return i8(a_sprite_getWidth(getCurrentSprite(Sprite, 0)));
}

int8_t z_sprite_getHeight(ZSprite* Sprite)
{
    return i8(a_sprite_getHeight(getCurrentSprite(Sprite, 0)));
}

#endif // ifndef ARDUINO

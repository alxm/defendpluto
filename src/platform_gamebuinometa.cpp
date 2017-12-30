/*
    Copyright 2017 Alex Margarit <alex@alxm.org>

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

#if Z_PLATFORM_GAMEBUINOMETA

#include "util_font.h"
#include "util_fps.h"
#include "util_graphics.h"
#include "util_input.h"
#include "util_random.h"
#include "data_gfx_palette.h"

typedef struct {
    Button index;
    bool pressed : 1;
    bool released : 1;
} ZButton;

static uint16_t g_frameCounter;
static ZButton g_buttons[Z_BUTTON_NUM];
static ZPalette g_paletteIndex;
static Color g_palettes[Z_PALETTE_NUM][Z_COLOR_NUM];

void z_platform_setup(void)
{
    g_frameCounter = 0;

    g_buttons[Z_BUTTON_UP].index = BUTTON_UP;
    g_buttons[Z_BUTTON_DOWN].index = BUTTON_DOWN;
    g_buttons[Z_BUTTON_LEFT].index = BUTTON_LEFT;
    g_buttons[Z_BUTTON_RIGHT].index = BUTTON_RIGHT;
    g_buttons[Z_BUTTON_A].index = BUTTON_A;
    g_buttons[Z_BUTTON_B].index = BUTTON_B;

    g_paletteIndex = Z_PALETTE_DEFAULT;

    const uint16_t palWidth = z_data_gfx_palette_buffer[0];

    for(int p = 0; p < Z_PALETTE_NUM; p++) {
        for(int c = 0; c < Z_COLOR_NUM; c++) {
            g_palettes[p][c] =
                (Color)z_data_gfx_palette_buffer[2 + p * palWidth + 1 + c];
        }
    }
}

void z_platform_tick(void)
{
    g_frameCounter++;

    for(uint8_t b = 0; b < Z_BUTTON_NUM; b++) {
        bool pressed = gb.buttons.repeat(g_buttons[b].index, 1);

        if(g_buttons[b].released) {
            if(!pressed) {
                g_buttons[b].released = false;
            }
        } else {
            g_buttons[b].pressed = pressed;
        }
    }
}

void z_platform_draw(void)
{
    //
}

uint16_t z_fps_getCounter(void)
{
    return g_frameCounter;
}

bool z_fps_isNthFrame(uint8_t N)
{
    return (g_frameCounter % N) == 0;
}

bool z_button_pressed(uint8_t Button)
{
    return g_buttons[Button].pressed;
}

void z_button_release(uint8_t Button)
{
    g_buttons[Button].pressed = false;
    g_buttons[Button].released = true;
}

void z_draw_fill(uint8_t Color)
{
    gb.display.setColor(g_palettes[g_paletteIndex][Color]);
    gb.display.fill();
}

void z_draw_rectangle(int16_t X, int16_t Y, int8_t W, int8_t H, uint8_t Color)
{
    gb.display.setColor(g_palettes[g_paletteIndex][Color]);
    gb.display.fillRect(X, Y, W, H);
}

void z_draw_pixel(int16_t X, int16_t Y, uint8_t Color)
{
    gb.display.setColor(g_palettes[g_paletteIndex][Color]);
    gb.display.drawPixel(X, Y);
}

void z_draw_circle(int16_t X, int16_t Y, uint8_t Radius, uint8_t Color)
{
    gb.display.setColor(g_palettes[g_paletteIndex][Color]);
    gb.display.drawCircle(X, Y, Radius);
}

void z_platform__loadSprite(ZSprite* Sprite, const uint16_t* Buffer, uint8_t NumFrames)
{
    Sprite->image = new Image(Buffer, NumFrames);
    Sprite->numFrames = NumFrames;
}

void z_sprite_blit(ZSprite* Sprite, int16_t X, int16_t Y, uint8_t Frame)
{
    Image* image = (Image*)Sprite->image;

    image->setFrame(Frame);
    gb.display.drawImage(X, Y, *image);
}

int8_t z_sprite_getWidth(ZSprite* Sprite)
{
    return i8(((Image*)Sprite->image)->width());
}

int8_t z_sprite_getHeight(ZSprite* Sprite)
{
    return i8(((Image*)Sprite->image)->height());
}

#endif // Z_PLATFORM_GAMEBUINOMETA
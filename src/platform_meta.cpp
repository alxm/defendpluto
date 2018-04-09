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

#include "data_gfx_palette.h"
#include "loop.h"
#include "util_fps.h"
#include "util_input.h"

#if Z_PLATFORM_META
typedef struct {
    Button index;
    bool pressed : 1;
    bool released : 1;
} ZButton;

typedef struct {
    Image image;
    uint8_t numFrames;
} ZSprite;

static ZButton g_buttons[Z_BUTTON_NUM];
static ZSprite g_sprites[Z_SPRITE_NUM];
static Color g_colors[Z_COLOR_NUM];

void setup(void)
{
    gb.begin();
    gb.setFrameRate(Z_FPS);

    g_buttons[Z_BUTTON_UP].index = BUTTON_UP;
    g_buttons[Z_BUTTON_DOWN].index = BUTTON_DOWN;
    g_buttons[Z_BUTTON_LEFT].index = BUTTON_LEFT;
    g_buttons[Z_BUTTON_RIGHT].index = BUTTON_RIGHT;
    g_buttons[Z_BUTTON_A].index = BUTTON_A;
    g_buttons[Z_BUTTON_B].index = BUTTON_B;
    g_buttons[Z_BUTTON_MENU].index = BUTTON_MENU;

    const uint16_t palWidth = z_data_gfx_palette_buffer[0];

    for(int c = 0; c < Z_COLOR_NUM; c++) {
        g_colors[c] =
            (Color)z_data_gfx_palette_buffer[
                Z_GAMEBUINO_IMAGE_HEADER_LEN + 1 * palWidth + 1 + c];
    }

    z_loop_setup();
}

void loop(void)
{
    if(!gb.update()) {
        return;
    }

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

    z_loop_tick();
    z_loop_draw();

    #if Z_DEBUG_STATS
        gb.display.setCursor(2, 2);
        gb.display.print(gb.getCpuLoad(), DEC);
        gb.display.setCursor(2, 8);
        gb.display.print(gb.getFreeRam(), DEC);
    #endif
}

bool z_button_pressed(ZButtonId Button)
{
    return g_buttons[Button].pressed;
}

void z_button_release(ZButtonId Button)
{
    g_buttons[Button].pressed = false;
    g_buttons[Button].released = true;
}

ZPixel* z_screen_getPixels(void)
{
    return gb.display._buffer;
}

void z_lights_put(int16_t X, int16_t Y, uint8_t Color)
{
    gb.lights.setColor(g_colors[Color]);
    gb.lights.drawPixel(X, Y);
}

void z_platform__loadSprite(ZSpriteId Sprite, const uint16_t* Buffer, uint8_t NumFrames)
{
    g_sprites[Sprite].image.init(Buffer);
    g_sprites[Sprite].numFrames = NumFrames;
}

ZPixel z_sprite_getTransparentColor(void)
{
    return 0xf81f;
}

ZPixel* z_sprite_getPixels(ZSpriteId Sprite, uint8_t Frame)
{
    g_sprites[Sprite].image.setFrame(Frame);
    return g_sprites[Sprite].image._buffer;
}

void z_sprite_blit(uint8_t Sprite, int16_t X, int16_t Y, uint8_t Frame)
{
    g_sprites[Sprite].image.setFrame(Frame);
    gb.display.drawImage(X, Y, g_sprites[Sprite].image);
}

int16_t z_sprite_getWidth(uint8_t Sprite)
{
    return g_sprites[Sprite].image.width();
}

int16_t z_sprite_getHeight(uint8_t Sprite)
{
    return g_sprites[Sprite].image.height();
}

uint8_t z_sprite_getNumFrames(uint8_t Sprite)
{
    return g_sprites[Sprite].numFrames;
}

void z_draw_fill(uint8_t Color)
{
    gb.display.setColor(g_colors[Color]);
    gb.display.fill();
}

void z_draw_rectangle(int16_t X, int16_t Y, int16_t W, int16_t H, uint8_t Color)
{
    gb.display.setColor(g_colors[Color]);
    gb.display.fillRect(X, Y, W, H);
}

void z_draw_pixel(int16_t X, int16_t Y, uint8_t Color)
{
    gb.display.setColor(g_colors[Color]);
    gb.display.drawPixel(X, Y);
}

void z_draw_circle(int16_t X, int16_t Y, int16_t Radius, uint8_t Color)
{
    gb.display.setColor(g_colors[Color]);
    gb.display.drawCircle(X, Y, Radius);
}

uint16_t z_fps_getCounter(void)
{
    return u16(gb.frameCount);
}

bool z_fps_isNthFrame(uint8_t N)
{
    return (gb.frameCount % N) == 0;
}
#endif // Z_PLATFORM_META

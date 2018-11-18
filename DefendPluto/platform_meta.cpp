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

#include "generated/data_gfx_palette.h"
#include "state.h"
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

typedef struct {
    const uint8_t* buffer;
    uint32_t size;
} ZSfx;

static ZButton g_buttons[Z_BUTTON_NUM];
static ZSprite g_sprites[Z_SPRITE_NUM];
static ZSfx g_sfx[Z_SFX_NUM];
static struct {
    Color color;
    int r, g, b;
} g_colors[Z_COLOR_NUM];

static inline void z_pixel_toRGB(ZPixel Pixel, int* R, int* G, int* B)
{
    *R = ((Pixel >> 11) & ((1 << 5) - 1)) << 3;
    *G = ((Pixel >> 5)  & ((1 << 6) - 1)) << 2;
    *B = ((Pixel >> 0)  & ((1 << 5) - 1)) << 3;
}

static inline ZPixel z_pixel_fromRGB(int Red, int Green, int Blue)
{
    return (ZPixel)
        (((Red >> 3) << 11) | ((Green >> 2) << 5)| ((Blue >> 3) << 0));
}

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
        ZPixel pixel = z_data_gfx_palette_buffer[
                Z_GAMEBUINO_IMAGE_HEADER_LEN + 1 * palWidth + 1 + c];

        g_colors[c].color = (Color)pixel;
        z_pixel_toRGB(pixel, &g_colors[c].r, &g_colors[c].g, &g_colors[c].b);
    }

    z_state_setup();
}

void loop(void)
{
    if(!gb.update()) {
        return;
    }

    for(uint8_t b = 0; b < Z_BUTTON_NUM; b++) {
        bool pressed = gb.buttons.repeat(g_buttons[b].index, 0);

        if(g_buttons[b].released) {
            if(!pressed) {
                g_buttons[b].released = false;
            }
        } else {
            g_buttons[b].pressed = pressed;
        }
    }

    z_state_tick();
    z_state_draw();

    #if Z_DEBUG_STATS
        gb.display.setColor(WHITE);
        gb.display.setCursor(2, 2);
        gb.display.print(gb.getCpuLoad(), DEC);
        gb.display.setCursor(2, 8);
        gb.display.print(gb.getFreeRam(), DEC);
        gb.display.setCursor(2, 14);
        gb.display.print(SOUND_CHANNELS, DEC);
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

static void prepLights(ZColorId BgColorId, ZColorId ColorId, int Alpha)
{
    ZPixel color = 0;

    if(ColorId != Z_COLOR_INVALID) {
        int r2 = g_colors[ColorId].r;
        int g2 = g_colors[ColorId].g;
        int b2 = g_colors[ColorId].b;

        if(BgColorId != Z_COLOR_INVALID) {
            int r1 = g_colors[BgColorId].r;
            int g1 = g_colors[BgColorId].g;
            int b1 = g_colors[BgColorId].b;

            color = z_pixel_fromRGB(r1 + (((r2 - r1) * Alpha) >> 8),
                                    g1 + (((g2 - g1) * Alpha) >> 8),
                                    b1 + (((b2 - b1) * Alpha) >> 8));
        } else {
            color = z_pixel_fromRGB((r2 * Alpha) >> 8,
                                    (g2 * Alpha) >> 8,
                                    (b2 * Alpha) >> 8);
        }
    } else if(BgColorId != Z_COLOR_INVALID) {
        color = (ZPixel)g_colors[BgColorId].color;
    }

    gb.lights.setColor((Color)color);
}

void z_platform_meta_fillLights(ZColorId BgColorId, ZColorId ColorId, int Alpha)
{
    prepLights(BgColorId, ColorId, Alpha);
    gb.lights.fill();
}

void z_platform_meta_drawLights(ZColorId ColorId, int Alpha, int16_t X, int16_t Y)
{
    prepLights(Z_COLOR_INVALID, ColorId, Alpha);
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

const ZPixel* z_sprite_getPixels(ZSpriteId Sprite, uint8_t Frame)
{
    g_sprites[Sprite].image.setFrame(Frame);
    return g_sprites[Sprite].image._buffer;
}

void z_sprite_blit(ZSpriteId Sprite, int16_t X, int16_t Y, uint8_t Frame)
{
    g_sprites[Sprite].image.setFrame(Frame);
    gb.display.drawImage(X, Y, g_sprites[Sprite].image);
}

int16_t z_sprite_getWidth(ZSpriteId Sprite)
{
    return g_sprites[Sprite].image.width();
}

int16_t z_sprite_getHeight(ZSpriteId Sprite)
{
    return g_sprites[Sprite].image.height();
}

uint8_t z_sprite_getNumFrames(ZSpriteId Sprite)
{
    return g_sprites[Sprite].numFrames;
}

void z_draw_fill(ZColorId ColorId)
{
    gb.display.setColor(g_colors[ColorId].color);
    gb.display.fill();
}

void z_draw_rectangle(int16_t X, int16_t Y, int16_t W, int16_t H, ZColorId ColorId)
{
    gb.display.setColor(g_colors[ColorId].color);
    gb.display.fillRect(X, Y, W, H);
}

void z_draw_pixel(int16_t X, int16_t Y, ZColorId ColorId)
{
    gb.display.setColor(g_colors[ColorId].color);
    gb.display.drawPixel(X, Y);
}

void z_draw_circle(int16_t X, int16_t Y, int16_t Radius, ZColorId ColorId)
{
    gb.display.setColor(g_colors[ColorId].color);
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

void z_platform__loadSfx(ZSfxId Sfx, const uint8_t* Buffer, uint32_t Size)
{
    g_sfx[Sfx].buffer = Buffer;
    g_sfx[Sfx].size = Size;
}

void z_sfx_play(ZSfxId Sfx)
{
    gb.sound.play(g_sfx[Sfx].buffer, g_sfx[Sfx].size);
}
#endif // Z_PLATFORM_META

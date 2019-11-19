/*
    Copyright 2017-2019 Alex Margarit <alex@alxm.org>
    This file is part of Defend Pluto, a video game.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "platform.h"

#include "state.h"
#include "util_fps.h"
#include "util_input.h"

#if Z_PLATFORM_A2X
typedef struct {
    FSample* sample;
    int channel;
} ZSfx;

static ZPixel g_colors[Z_COLOR_NUM];
static FSprite* g_sprites[Z_SPRITE_NUM];
static ZSfx g_sfx[Z_SFX_NUM];
static FButton* g_buttons[Z_BUTTON_NUM];

void f_init(void)
{
    f_init_app("Defend Pluto", "alxm", 0, 3, 0);
    f_init_fps(Z_FPS, Z_FPS);
}

void f_main(void)
{
    F_STATE_INIT
    {
        g_buttons[Z_BUTTON_UP] = f_button_new();
        f_button_bindKey(g_buttons[Z_BUTTON_UP], F_KEY_UP);
        f_button_bindButton(g_buttons[Z_BUTTON_UP], NULL, F_BUTTON_UP);

        g_buttons[Z_BUTTON_DOWN] = f_button_new();
        f_button_bindKey(g_buttons[Z_BUTTON_DOWN], F_KEY_DOWN);
        f_button_bindButton(g_buttons[Z_BUTTON_DOWN], NULL, F_BUTTON_DOWN);

        g_buttons[Z_BUTTON_LEFT] = f_button_new();
        f_button_bindKey(g_buttons[Z_BUTTON_LEFT], F_KEY_LEFT);
        f_button_bindButton(g_buttons[Z_BUTTON_LEFT], NULL, F_BUTTON_LEFT);
        f_button_bindButton(g_buttons[Z_BUTTON_LEFT], NULL, F_BUTTON_L);

        g_buttons[Z_BUTTON_RIGHT] = f_button_new();
        f_button_bindKey(g_buttons[Z_BUTTON_RIGHT], F_KEY_RIGHT);
        f_button_bindButton(g_buttons[Z_BUTTON_RIGHT], NULL, F_BUTTON_RIGHT);
        f_button_bindButton(g_buttons[Z_BUTTON_RIGHT], NULL, F_BUTTON_R);

        g_buttons[Z_BUTTON_A] = f_button_new();
        f_button_bindKey(g_buttons[Z_BUTTON_A], F_KEY_SPACE);
        f_button_bindKey(g_buttons[Z_BUTTON_A], F_KEY_Z);
        f_button_bindButton(g_buttons[Z_BUTTON_A], NULL, F_BUTTON_A);

        g_buttons[Z_BUTTON_B] = f_button_new();
        f_button_bindKey(g_buttons[Z_BUTTON_B], F_KEY_X);
        f_button_bindButton(g_buttons[Z_BUTTON_B], NULL, F_BUTTON_B);

        g_buttons[Z_BUTTON_MENU] = f_button_new();
        f_button_bindKey(g_buttons[Z_BUTTON_MENU], F_KEY_ENTER);
        f_button_bindButton(g_buttons[Z_BUTTON_MENU], NULL, F_BUTTON_START);

        FSprite* pal = f_sprite_newFromPng("assets/gfx/palette.png", 0, 0, 0, 0);

        for(ZColorId c = 0; c < Z_COLOR_NUM; c++) {
            g_colors[c] = f_sprite_pixelsGetValue(pal, 0, 1 + c, 1);
        }

        f_sprite_free(pal);

        z_state_setup();
    }

    F_STATE_TICK
    {
        z_state_tick();
    }

    F_STATE_DRAW
    {
        z_state_draw();
    }

    F_STATE_FREE
    {
        for(int b = 0; b < Z_BUTTON_NUM; b++) {
            f_button_free(g_buttons[b]);
        }

        for(int s = 0; s < Z_SPRITE_NUM; s++) {
            f_sprite_free(g_sprites[s]);
        }

        for(int s = 0; s < Z_SFX_NUM; s++) {
            f_sample_free(g_sfx[s].sample);
        }
    }
}

bool z_button_pressed(ZButtonId Button)
{
    return f_button_pressGet(g_buttons[Button]);
}

void z_button_release(ZButtonId Button)
{
    f_button_pressClear(g_buttons[Button]);
}

ZPixel* z_screen_getPixels(void)
{
    return f_screen_pixelsGetBuffer();
}

void z_platform__loadSprite(ZSpriteId Sprite, const char* Path)
{
    g_sprites[Sprite] = f_sprite_newFromPng(Path, 0, 0, 0, 0);
}

ZPixel z_sprite_getTransparentColor(void)
{
    return f_color_pixelFromHex(0xff00ff);
}

const ZPixel* z_sprite_getPixels(ZSpriteId Sprite, uint8_t Frame)
{
    return f_sprite_pixelsGetBuffer(g_sprites[Sprite], Frame);
}

void z_sprite_blit(ZSpriteId Sprite, int16_t X, int16_t Y, uint8_t Frame)
{
    f_sprite_blit(g_sprites[Sprite], Frame, X, Y);
}

int16_t z_sprite_getWidth(ZSpriteId Sprite)
{
    return i16(f_sprite_sizeGetWidth(g_sprites[Sprite]));
}

int16_t z_sprite_getHeight(ZSpriteId Sprite)
{
    return i16(f_sprite_sizeGetHeight(g_sprites[Sprite]));
}

uint8_t z_sprite_getNumFrames(ZSpriteId Sprite)
{
    return u8(f_sprite_framesNumGet(g_sprites[Sprite]));
}

void z_draw_fill(ZColorId ColorId)
{
    f_color_colorSetPixel(g_colors[ColorId]);
    f_draw_fill();
}

void z_draw_rectangle(int16_t X, int16_t Y, int16_t W, int16_t H, ZColorId ColorId)
{
    f_color_colorSetPixel(g_colors[ColorId]);
    f_draw_rectangle(X, Y, W, H);
}

void z_draw_pixel(int16_t X, int16_t Y, ZColorId ColorId)
{
    f_color_colorSetPixel(g_colors[ColorId]);
    f_draw_pixel(X, Y);
}

void z_draw_circle(int16_t X, int16_t Y, int16_t Radius, ZColorId ColorId)
{
    f_color_push();

    f_color_colorSetPixel(g_colors[ColorId]);
    f_color_fillDrawSet(false);
    f_draw_circle(X, Y, Radius);

    f_color_pop();
}

uint16_t z_fps_getCounter(void)
{
    return u16(f_fps_ticksGet());
}

bool z_fps_isNthFrame(uint8_t N)
{
    return f_fps_ticksNth(N);
}

void z_platform__loadSfx(ZSfxId Sfx, const char* Path)
{
    g_sfx[Sfx].sample = f_sample_new(Path);
    g_sfx[Sfx].channel = f_channel_new();
}

void z_sfx_play(ZSfxId Sfx)
{
    f_channel_play(g_sfx[Sfx].channel, g_sfx[Sfx].sample, F_CHANNEL_PLAY_RESTART);
}
#endif // Z_PLATFORM_A2X

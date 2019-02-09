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
static AButton* g_buttons[Z_BUTTON_NUM];
static ASpriteFrames* g_sprites[Z_SPRITE_NUM];
static struct {
    ASample* sample;
    int channel;
} g_sfx[Z_SFX_NUM];
static ZPixel g_colors[Z_COLOR_NUM];

A_STATE(run)
{
    A_STATE_INIT
    {
        g_buttons[Z_BUTTON_UP] = a_button_new();
        a_button_bind(g_buttons[Z_BUTTON_UP], A_KEY_UP);
        a_button_bind(g_buttons[Z_BUTTON_UP], A_BUTTON_UP);

        g_buttons[Z_BUTTON_DOWN] = a_button_new();
        a_button_bind(g_buttons[Z_BUTTON_DOWN], A_KEY_DOWN);
        a_button_bind(g_buttons[Z_BUTTON_DOWN], A_BUTTON_DOWN);

        g_buttons[Z_BUTTON_LEFT] = a_button_new();
        a_button_bind(g_buttons[Z_BUTTON_LEFT], A_KEY_LEFT);
        a_button_bind(g_buttons[Z_BUTTON_LEFT], A_BUTTON_LEFT);

        g_buttons[Z_BUTTON_RIGHT] = a_button_new();
        a_button_bind(g_buttons[Z_BUTTON_RIGHT], A_KEY_RIGHT);
        a_button_bind(g_buttons[Z_BUTTON_RIGHT], A_BUTTON_RIGHT);

        g_buttons[Z_BUTTON_A] = a_button_new();
        a_button_bind(g_buttons[Z_BUTTON_A], A_KEY_Z);
        a_button_bind(g_buttons[Z_BUTTON_A], A_BUTTON_A);

        g_buttons[Z_BUTTON_B] = a_button_new();
        a_button_bind(g_buttons[Z_BUTTON_B], A_KEY_X);
        a_button_bind(g_buttons[Z_BUTTON_B], A_BUTTON_B);

        g_buttons[Z_BUTTON_MENU] = a_button_new();
        a_button_bind(g_buttons[Z_BUTTON_MENU], A_KEY_ENTER);
        a_button_bind(g_buttons[Z_BUTTON_MENU], A_BUTTON_START);

        ASprite* pal = a_sprite_newFromPng("assets/gfx/palette.png");

        for(ZColorId c = 0; c < Z_COLOR_NUM; c++) {
            g_colors[c] = a_sprite_pixelsGetPixel(pal, 1 + c, 1);
        }

        a_sprite_free(pal);

        z_state_setup();
    }

    A_STATE_TICK
    {
        z_state_tick();
    }

    A_STATE_DRAW
    {
        z_state_draw();
    }

    A_STATE_FREE
    {
        for(ZButtonId b = 0; b < Z_BUTTON_NUM; b++) {
            a_button_free(g_buttons[b]);
        }

        for(ZSpriteId s = 0; s < Z_SPRITE_NUM; s++) {
            a_spriteframes_free(g_sprites[s], true);
        }

        for(ZSfxId s = 0; s < Z_SFX_NUM; s++) {
            a_sample_free(g_sfx[s].sample);
        }
    }
}

A_MAIN
{
    #if Z_DEBUG_GENERATE_LUT
        {
            uint8_t angle = 0;
            ZFix lastRatio = 0;

            printf("PROGMEM static const uint8_t g_atanAngles[Z_FIX_ONE] = {");

            for(ZFix refRatio = 0; refRatio < Z_FIX_ONE; refRatio++) {
                if(refRatio % 16 == 0) {
                    printf("\n    ");
                }

                ZFix ratio = zf(a_fix_div(a_fix_sin(angle), a_fix_cos(angle))
                                    / (A_FIX_ONE / Z_FIX_ONE));

                while(ratio < refRatio) {
                    angle++;
                    lastRatio = ratio;
                    ratio = zf(a_fix_div(a_fix_sin(angle), a_fix_cos(angle))
                                    / (A_FIX_ONE / Z_FIX_ONE));
                }

                ZFix diff1 = zf(refRatio - lastRatio);
                ZFix diff2 = zf(ratio - refRatio);

                if(diff2 <= diff1) {
                    printf("%d, ",
                           angle / (A_MATH_ANGLES_NUM / Z_ANGLES_NUM));
                } else {
                    printf("%d, ",
                           (unsigned)
                            (angle - 1) / (A_MATH_ANGLES_NUM / Z_ANGLES_NUM));
                }
            }

            printf("\n};\n");
        }

        printf("\n");

        {
            printf("PROGMEM const ZFix z_fix__sin[Z_ANGLES_NUM] = {");

            for(unsigned a = 0;
                a < A_MATH_ANGLES_NUM;
                a += A_MATH_ANGLES_NUM / Z_ANGLES_NUM) {

                if(a % 16 == 0) {
                    printf("\n    ");
                }

                printf("%d, ", a_fix_sin(a) / (A_FIX_ONE / Z_FIX_ONE));
            }

            printf("\n};\n");
        }
    #else
        a_state_init(1);
        a_state_new(0, run, A_CONFIG_APP_TITLE);
        a_state_push(0);
    #endif
}

bool z_button_pressed(ZButtonId Button)
{
    return a_button_pressGet(g_buttons[Button]);
}

void z_button_release(ZButtonId Button)
{
    a_button_pressClear(g_buttons[Button]);
}

ZPixel* z_screen_getPixels(void)
{
    return a_screen_pixelsGetBuffer();
}

void z_platform__loadSprite(ZSpriteId Sprite, const char* Path)
{
    g_sprites[Sprite] = a_spriteframes_newFromPng(Path, 0, 0);
}

ZPixel z_sprite_getTransparentColor(void)
{
    return a_sprite_colorKeyGet();
}

static ASprite* getCurrentSprite(ZSpriteId Sprite, uint8_t Frame)
{
    return a_spriteframes_getByIndex(g_sprites[Sprite], Frame);
}

const ZPixel* z_sprite_getPixels(ZSpriteId Sprite, uint8_t Frame)
{
    return a_sprite_pixelsGetBuffer(getCurrentSprite(Sprite, Frame));
}

void z_sprite_blit(ZSpriteId Sprite, int16_t X, int16_t Y, uint8_t Frame)
{
    a_sprite_blit(getCurrentSprite(Sprite, Frame), X, Y);
}

int16_t z_sprite_getWidth(ZSpriteId Sprite)
{
    return i16(a_sprite_sizeGetWidth(getCurrentSprite(Sprite, 0)));
}

int16_t z_sprite_getHeight(ZSpriteId Sprite)
{
    return i16(a_sprite_sizeGetHeight(getCurrentSprite(Sprite, 0)));
}

uint8_t z_sprite_getNumFrames(ZSpriteId Sprite)
{
    return u8(a_spriteframes_framesNumGet(g_sprites[Sprite]));
}

void z_draw_fill(ZColorId ColorId)
{
    a_color_baseSetPixel(g_colors[ColorId]);
    a_draw_fill();
}

void z_draw_rectangle(int16_t X, int16_t Y, int16_t W, int16_t H, ZColorId ColorId)
{
    a_color_baseSetPixel(g_colors[ColorId]);
    a_draw_rectangle(X, Y, W, H);
}

void z_draw_pixel(int16_t X, int16_t Y, ZColorId ColorId)
{
    a_color_baseSetPixel(g_colors[ColorId]);
    a_draw_pixel(X, Y);
}

void z_draw_circle(int16_t X, int16_t Y, int16_t Radius, ZColorId ColorId)
{
    a_color_push();

    a_color_baseSetPixel(g_colors[ColorId]);
    a_color_fillDrawSet(false);
    a_draw_circle(X, Y, Radius);

    a_color_pop();
}

uint16_t z_fps_getCounter(void)
{
    return u16(a_fps_ticksGet());
}

bool z_fps_isNthFrame(uint8_t N)
{
    return a_fps_ticksNth(N);
}

void z_platform__loadSfx(ZSfxId Sfx, const char* Path)
{
    g_sfx[Sfx].sample = a_sample_new(Path);
    g_sfx[Sfx].channel = a_channel_new();
}

void z_sfx_play(ZSfxId Sfx)
{
    a_channel_play(g_sfx[Sfx].channel, g_sfx[Sfx].sample, A_CHANNEL_RESTART);
}
#endif // Z_PLATFORM_A2X

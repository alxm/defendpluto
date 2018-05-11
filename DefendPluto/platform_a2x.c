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

#include "state.h"
#include "util_fps.h"
#include "util_input.h"

#if Z_PLATFORM_A2X
static AInputButton* g_buttons[Z_BUTTON_NUM];
static ASpriteFrames* g_sprites[Z_SPRITE_NUM];
static ASfx * g_sfx[Z_SFX_NUM];
static ZPixel g_colors[Z_COLOR_NUM];

A_SETUP
{
    a_settings_set("app.title", "Defend Pluto");
    a_settings_set("app.version", "Beta 1");
    a_settings_set("app.author", "alxm");
    a_settings_set("app.output.on", "yes");
    a_settings_set("app.output.verbose", "yes");
    a_settings_set("video.color.border", "0x111523");
    a_settings_set("video.width", A_STRINGIFY(Z_SCREEN_W));
    a_settings_set("video.height", A_STRINGIFY(Z_SCREEN_H));
    a_settings_set("fps.tick", A_STRINGIFY(Z_FPS));
    a_settings_set("fps.draw", A_STRINGIFY(Z_FPS));
}

A_STATE(run)
{
    A_STATE_INIT
    {
        g_buttons[Z_BUTTON_UP] = a_button_new("key.up gamepad.b.up");
        g_buttons[Z_BUTTON_DOWN] = a_button_new("key.down gamepad.b.down");
        g_buttons[Z_BUTTON_LEFT] = a_button_new("key.left gamepad.b.left");
        g_buttons[Z_BUTTON_RIGHT] = a_button_new("key.right gamepad.b.right");
        g_buttons[Z_BUTTON_A] = a_button_new("key.z gamepad.b.a");
        g_buttons[Z_BUTTON_B] = a_button_new("key.x gamepad.b.b");
        g_buttons[Z_BUTTON_MENU] = a_button_new("key.enter gamepad.b.start");

        ASprite* pal = a_sprite_newFromFile("assets/gfx/palette.png");

        for(ZColorId c = 0; c < Z_COLOR_NUM; c++) {
            g_colors[c] = a_sprite_getPixel(pal, 1 + c, 1);
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
        a_state_new("run", run, "", "");
        a_state_push("run");
    #endif
}

bool z_button_pressed(ZButtonId Button)
{
    return a_button_getPressed(g_buttons[Button]);
}

void z_button_release(ZButtonId Button)
{
    a_button_release(g_buttons[Button]);
}

ZPixel* z_screen_getPixels(void)
{
    return a_screen_getPixels();
}

void z_platform__loadSprite(ZSpriteId Sprite, const char* Path)
{
    g_sprites[Sprite] = a_spriteframes_newFromFile(Path, 0);
}

ZPixel z_sprite_getTransparentColor(void)
{
    return a_sprite_getColorKey();
}

static ASprite* getCurrentSprite(ZSpriteId Sprite, uint8_t Frame)
{
    return a_spriteframes_getByIndex(g_sprites[Sprite], Frame);
}

ZPixel* z_sprite_getPixels(ZSpriteId Sprite, uint8_t Frame)
{
    return a_sprite_getPixels(getCurrentSprite(Sprite, Frame));
}

void z_sprite_blit(ZSpriteId Sprite, int16_t X, int16_t Y, uint8_t Frame)
{
    a_sprite_blit(getCurrentSprite(Sprite, Frame), X, Y);
}

int16_t z_sprite_getWidth(ZSpriteId Sprite)
{
    return i16(a_sprite_getWidth(getCurrentSprite(Sprite, 0)));
}

int16_t z_sprite_getHeight(ZSpriteId Sprite)
{
    return i16(a_sprite_getHeight(getCurrentSprite(Sprite, 0)));
}

uint8_t z_sprite_getNumFrames(ZSpriteId Sprite)
{
    return u8(a_spriteframes_getNum(g_sprites[Sprite]));
}

void z_draw_fill(ZColorId Color)
{
    a_pixel_setPixel(g_colors[Color]);
    a_draw_fill();
}

void z_draw_rectangle(int16_t X, int16_t Y, int16_t W, int16_t H, ZColorId Color)
{
    a_pixel_setPixel(g_colors[Color]);
    a_draw_rectangle(X, Y, W, H);
}

void z_draw_pixel(int16_t X, int16_t Y, ZColorId Color)
{
    a_pixel_setPixel(g_colors[Color]);
    a_draw_pixel(X, Y);
}

void z_draw_circle(int16_t X, int16_t Y, int16_t Radius, ZColorId Color)
{
    a_pixel_push();

    a_pixel_setPixel(g_colors[Color]);
    a_pixel_setFillDraw(false);
    a_draw_circle(X, Y, Radius);

    a_pixel_pop();
}

uint16_t z_fps_getCounter(void)
{
    return u16(a_fps_getCounter());
}

bool z_fps_isNthFrame(uint8_t N)
{
    return a_fps_isNthFrame(N);
}

void z_platform__loadSfx(ZSfxId Sfx, const char* Path)
{
    g_sfx[Sfx] = a_sfx_new(Path);
}

void z_sfx_play(ZSfxId Sfx)
{
    a_sfx_play(g_sfx[Sfx], A_SFX_RESTART);
}
#endif // Z_PLATFORM_A2X

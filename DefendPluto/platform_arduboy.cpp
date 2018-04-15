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
#include "state.h"
#include "util_font.h"
#include "util_fps.h"
#include "util_input.h"

#if Z_PLATFORM_ARDUBOY
typedef struct {
    const uint8_t* image;
    const uint8_t* mask;
    uint8_t numFrames;
} ZSprite;

static Arduboy2Base g_arduboy;

#define buttonFields(Prefix)   \
    bool Prefix##Pressed : 1;  \
    bool Prefix##Released : 1;

static struct {
    buttonFields(up)
    buttonFields(down)
    buttonFields(left)
    buttonFields(right)
    buttonFields(a)
    buttonFields(b)
} g_buttons;

static ZSprite g_sprites[Z_SPRITE_NUM];

void setup(void)
{
    g_arduboy.begin();
    g_arduboy.setFrameRate(Z_FPS);

    z_state_setup();
}

void loop(void)
{
    #if Z_DEBUG_STATS
        #define Z_NEXT_FRAME nextFrameDEV
    #else
        #define Z_NEXT_FRAME nextFrame
    #endif

    if(!g_arduboy.Z_NEXT_FRAME()) {
        return;
    }

    g_arduboy.pollButtons();

    #define pollButton(Prefix, Code)                \
    {                                               \
        bool pressed = g_arduboy.pressed(Code);     \
                                                    \
        if(g_buttons.Prefix##Released) {            \
            if(!pressed) {                          \
                g_buttons.Prefix##Released = false; \
            }                                       \
        } else {                                    \
            g_buttons.Prefix##Pressed = pressed;    \
        }                                           \
    }

    pollButton(up, UP_BUTTON);
    pollButton(down, DOWN_BUTTON);
    pollButton(left, LEFT_BUTTON);
    pollButton(right, RIGHT_BUTTON);
    pollButton(a, A_BUTTON);
    pollButton(b, B_BUTTON);

    z_state_tick();
    z_state_draw();

    #if Z_DEBUG_STATS
        extern uint8_t _end;

        uint16_t x;
        int16_t unusedBytes = 0;
        uint8_t here;

        for(x = u16(&_end); x < u16(&here) && *(uint8_t*)x == 0x55; x++) {
            unusedBytes++;
        }

        z_font_int(g_arduboy.cpuLoad(), 2, 2, Z_FONT_FACE_LCD, Z_FONT_ALIGN_L);
        z_font_int(unusedBytes, 2, 10, Z_FONT_FACE_LCD, Z_FONT_ALIGN_L);
    #endif

    g_arduboy.display();
}

bool z_button_pressed(ZButtonId Button)
{
    #define buttonCase(Id, Prefix)            \
        case Id:                              \
            return g_buttons.Prefix##Pressed; \

    switch(Button) {
        buttonCase(Z_BUTTON_UP, up)
        buttonCase(Z_BUTTON_DOWN, down)
        buttonCase(Z_BUTTON_LEFT, left)
        buttonCase(Z_BUTTON_RIGHT, right)
        buttonCase(Z_BUTTON_A, a)
        buttonCase(Z_BUTTON_B, b)

        default:
            return false;
    }

    #undef buttonCase
}

void z_button_release(ZButtonId Button)
{
    #define buttonCase(Id, Prefix)             \
        case Id:                               \
            g_buttons.Prefix##Pressed = false; \
            g_buttons.Prefix##Released = true; \
            break;

    switch(Button) {
        buttonCase(Z_BUTTON_UP, up)
        buttonCase(Z_BUTTON_DOWN, down)
        buttonCase(Z_BUTTON_LEFT, left)
        buttonCase(Z_BUTTON_RIGHT, right)
        buttonCase(Z_BUTTON_A, a)
        buttonCase(Z_BUTTON_B, b)

        default:
            break;
    }

    #undef buttonCase
}

void z_platform__loadSprite(ZSpriteId Sprite, const uint8_t* Buffer, uint8_t NumFrames)
{
    g_sprites[Sprite].image = Buffer;
    g_sprites[Sprite].mask = Buffer
                             + 2
                             + NumFrames
                                * z_sprite_getWidth(Sprite)
                                * ((z_sprite_getHeight(Sprite) + 7) / 8);
    g_sprites[Sprite].numFrames = NumFrames;
}

void z_sprite_blit(ZSpriteId Sprite, int16_t X, int16_t Y, uint8_t Frame)
{
    Sprites::drawExternalMask(X,
                              Y,
                              g_sprites[Sprite].image,
                              g_sprites[Sprite].mask,
                              Frame,
                              Frame);
}

int16_t z_sprite_getWidth(ZSpriteId Sprite)
{
    return pgm_read_byte(g_sprites[Sprite].image);
}

int16_t z_sprite_getHeight(ZSpriteId Sprite)
{
    return pgm_read_byte(g_sprites[Sprite].image + 1);
}

uint8_t z_sprite_getNumFrames(ZSpriteId Sprite)
{
    return g_sprites[Sprite].numFrames;
}

static inline uint8_t getColor(ZColorId Color)
{
    return Color < Z_COLORS_WHITE_START ? BLACK : WHITE;
}

void z_draw_fill(ZColorId Color)
{
    g_arduboy.fillScreen(getColor(Color));
}

void z_draw_rectangle(int16_t X, int16_t Y, int16_t W, int16_t H, ZColorId Color)
{
    g_arduboy.fillRect(X, Y, u8(W), u8(H), getColor(Color));
}

void z_draw_pixel(int16_t X, int16_t Y, ZColorId Color)
{
    g_arduboy.drawPixel(X, Y, getColor(Color));
}

void z_draw_circle(int16_t X, int16_t Y, int16_t Radius, ZColorId Color)
{
    g_arduboy.drawCircle(X, Y, u8(Radius), getColor(Color));
}

uint16_t z_fps_getCounter(void)
{
    return g_arduboy.frameCount;
}

bool z_fps_isNthFrame(uint8_t N)
{
    return g_arduboy.everyXFrames(N);
}
#endif // Z_PLATFORM_ARDUBOY

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

#if Z_PLATFORM_ARDUBOY
#include <Arduboy2.h>
#include <Sprites.h>
#include "util_font.h"
#include "util_fps.h"
#include "util_graphics.h"
#include "util_input.h"

typedef struct {
    const uint8_t* image;
    const uint8_t* mask;
    uint8_t numFrames;
} ZSprite;

extern Arduboy2Base g_arduboy;

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

void z_platform_setup(void)
{
    #if Z_DEBUG_STATS
        extern uint8_t _end;

        uint16_t x;
        uint8_t here;

        for(x = u16(&_end); x < u16(&here); x++) {
            *(uint8_t*)x = 0x55;
        }
    #endif
}

void z_platform_tick(void)
{
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
}

void z_platform_draw(void)
{
    #if Z_DEBUG_STATS
        extern uint8_t _end;

        uint16_t x;
        int16_t unusedBytes = 0;
        uint8_t here;

        for(x = u16(&_end); x < u16(&here) && *(uint8_t*)x == 0x55; x++) {
            unusedBytes++;
        }

        z_font_int(g_arduboy.cpuLoad(), 112, 2, Z_FONT_FACE_NUMBERS);
        z_font_int(unusedBytes, 112, 10, Z_FONT_FACE_NUMBERS);
    #endif
}

uint16_t z_fps_getCounter(void)
{
    return g_arduboy.frameCount;
}

bool z_fps_isNthFrame(uint8_t N)
{
    return g_arduboy.everyXFrames(N);
}

bool z_button_pressed(uint8_t Button)
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

void z_button_release(uint8_t Button)
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
    }

    #undef buttonCase
}

void z_draw_fill(uint8_t Color)
{
    g_arduboy.fillScreen(Color ? WHITE : BLACK);
}

void z_draw_rectangle(int16_t X, int16_t Y, int8_t W, int8_t H, uint8_t Color)
{
    g_arduboy.fillRect(X, Y, W, H, Color ? WHITE : BLACK);
}

void z_draw_pixel(int16_t X, int16_t Y, uint8_t Color)
{
    g_arduboy.drawPixel(X, Y, Color ? WHITE : BLACK);
}

void z_draw_circle(int16_t X, int16_t Y, uint8_t Radius, uint8_t Color)
{
    g_arduboy.drawCircle(X, Y, Radius, Color ? WHITE : BLACK);
}

void z_platform__loadSprite(uint8_t Sprite, const uint8_t* Buffer, uint8_t NumFrames)
{
    g_sprites[Sprite].image = Buffer;
    g_sprites[Sprite].mask = Buffer
                             + 2
                             + NumFrames
                                * z_sprite_getWidth(Sprite)
                                * ((z_sprite_getHeight(Sprite) + 7) / 8);
    g_sprites[Sprite].numFrames = NumFrames;
}

void z_sprite_blit(uint8_t Sprite, int16_t X, int16_t Y, uint8_t Frame)
{
    Sprites::drawExternalMask(X,
                              Y,
                              g_sprites[Sprite].image,
                              g_sprites[Sprite].mask,
                              Frame,
                              Frame);
}

int16_t z_sprite_getWidth(uint8_t Sprite)
{
    return pgm_read_byte(g_sprites[Sprite].image);
}

int16_t z_sprite_getHeight(uint8_t Sprite)
{
    return pgm_read_byte(g_sprites[Sprite].image + 1);
}

uint8_t z_sprite_getNumFrames(uint8_t Sprite)
{
    return g_sprites[Sprite].numFrames;
}
#endif // Z_PLATFORM_ARDUBOY

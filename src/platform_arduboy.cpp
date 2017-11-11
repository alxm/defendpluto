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

#include "platform.h"
#include <Sprites.h>
#include "util_font.h"
#include "util_fps.h"
#include "util_graphics.h"
#include "util_input.h"

extern Arduboy2Base g_arduboy;
ZControls z_controls;

void z_platform_setup(void)
{
    z_controls.up = UP_BUTTON;
    z_controls.down = DOWN_BUTTON;
    z_controls.left = LEFT_BUTTON;
    z_controls.right = RIGHT_BUTTON;
    z_controls.a = A_BUTTON;
    z_controls.b = B_BUTTON;

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
    //
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

bool z_button_pressed(ZButton Button)
{
    return g_arduboy.pressed(Button);
}

void z_draw_fill(uint8_t Color)
{
    g_arduboy.fillScreen(Color ? WHITE : BLACK);
}

void z_draw_rectangle(int8_t X, int8_t Y, int8_t W, int8_t H, uint8_t Color)
{
    g_arduboy.fillRect(X, Y, W, H, Color ? WHITE : BLACK);
}

void z_draw_pixel(int8_t X, int8_t Y, uint8_t Color)
{
    g_arduboy.drawPixel(X, Y, Color ? WHITE : BLACK);
}

void z_draw_circle(int8_t X, int8_t Y, uint8_t Radius, uint8_t Color)
{
    g_arduboy.drawCircle(X, Y, Radius, Color ? WHITE : BLACK);
}

void z_platform__loadSprite(ZSprite* Sprite, const uint8_t* Image, const uint8_t* Mask, uint8_t NumFrames)
{
    Sprite->image = Image;
    Sprite->mask = Mask;
    Sprite->numFrames = NumFrames;
}

void z_sprite_blit(ZSprite* Sprite, int8_t X, int8_t Y, uint8_t Frame)
{
    Sprites::drawExternalMask(X, Y, Sprite->image, Sprite->mask, Frame, Frame);
}

int8_t z_sprite_getWidth(ZSprite* Sprite)
{
    return i8(pgm_read_byte(Sprite->image));
}

int8_t z_sprite_getHeight(ZSprite* Sprite)
{
    return i8(pgm_read_byte(Sprite->image + 1));
}

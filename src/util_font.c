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
#include "util_font.h"
#include "util_graphics.h"
#include "data_gfx_font_num.h"
#include "data_gfx_font_alphanum.h"

typedef enum {
    Z_FONT_FLAG_NUMERIC = 0x1,
    Z_FONT_FLAG_ALPHA_L = 0x2,
    Z_FONT_FLAG_ALPHA_U = 0x4,
} ZFontFlag;

typedef struct {
    ZSprite sprites;
    uint8_t flags;
} ZFont;

ZFont g_fonts[Z_FONT_FACE_NUM];

void z_font_setup(void)
{
    #define loadFont(Index, Id, Flags)              \
        z_sprite_load(&g_fonts[Index].sprites, Id); \
        g_fonts[Index].flags = Flags;

    loadFont(Z_FONT_FACE_NUMBERS, font_num, Z_FONT_FLAG_NUMERIC);
    loadFont(Z_FONT_FACE_ALPHANUM,
             font_alphanum,
             Z_FONT_FLAG_NUMERIC
           | Z_FONT_FLAG_ALPHA_U
           | Z_FONT_FLAG_ALPHA_L);
    loadFont(Z_FONT_FACE_ALPHANUM_OUTLINE,
             font_alphanum_outline,
             Z_FONT_FLAG_NUMERIC
           | Z_FONT_FLAG_ALPHA_U
           | Z_FONT_FLAG_ALPHA_L);
}

void z_font_text(const char* Text, int8_t X, int8_t Y, uint8_t Font)
{
    uint8_t flags = g_fonts[Font].flags;
    ZSprite* sprite = &g_fonts[Font].sprites;

    while(*Text != '\0') {
        char c = *Text;
        char frame = 0;

        if(c >= 48 && c < 58) {
            if(flags & Z_FONT_FLAG_NUMERIC) {
                // numbers
                frame = (char)(1 + c - 48);
            }
        } else if(c >= 65 && c < 91) {
            if(flags & Z_FONT_FLAG_ALPHA_U) {
                // uppercase
                frame = (char)(1 + 10 + c - 65);
            }
        } else if(c >= 97 && c < 123) {
            if(flags & Z_FONT_FLAG_ALPHA_L) {
                // lowercase
                frame = (char)(1 + 10 + 26 + c - 97);
            } else if(flags & Z_FONT_FLAG_ALPHA_U) {
                // fallback to uppercase
                frame = (char)(1 + 10 + c - 97);
            }
        }

        z_sprite_blit(sprite, X, Y, u8(frame));
        X = i8(X + z_sprite_getWidth(sprite) + 1);

        Text++;
    }
}

void z_font_int(int16_t Number, int8_t X, int8_t Y, uint8_t Font)
{
    #define Z_BUFFER_SIZE 6
    char buffer[Z_BUFFER_SIZE];
    int8_t index = Z_BUFFER_SIZE;
    int16_t n = Number < 0 ? 0 : Number;

    buffer[--index] = '\0';

    do {
        buffer[--index] = (char)(48 + (n % 10));
        n /= 10;
    } while(n > 0);

    z_font_text(buffer + index, X, Y, Font);
}

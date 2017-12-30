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
#include "util_screen.h"
#include "util_str.h"
#include "data_gfx_font_num.h"
#include "data_gfx_font_alphanum.h"
#include "data_gfx_font_alphanum_outline.h"
#include "data_gfx_font_alphanum_outline_yellow.h"

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
    loadFont(Z_FONT_FACE_ALPHANUM_OUTLINE_RED,
             font_alphanum_outline,
             Z_FONT_FLAG_NUMERIC
           | Z_FONT_FLAG_ALPHA_U
           | Z_FONT_FLAG_ALPHA_L);
    loadFont(Z_FONT_FACE_ALPHANUM_OUTLINE_YELLOW,
             font_alphanum_outline_yellow,
             Z_FONT_FLAG_NUMERIC
           | Z_FONT_FLAG_ALPHA_U
           | Z_FONT_FLAG_ALPHA_L);
}

static int16_t drawChar(char Char, int16_t X, int16_t Y, uint8_t Flags, ZSprite* Sprite, int8_t CharWidth)
{
    char frame = 0;

    if(Char >= 48 && Char < 58) {
        if(Flags & Z_FONT_FLAG_NUMERIC) {
            // numbers
            frame = (char)(1 + Char - 48);
        }
    } else if(Char >= 65 && Char < 91) {
        if(Flags & Z_FONT_FLAG_ALPHA_U) {
            // uppercase
            frame = (char)(1 + 10 + Char - 65);
        }
    } else if(Char >= 97 && Char < 123) {
        if(Flags & Z_FONT_FLAG_ALPHA_L) {
            // lowercase
            frame = (char)(1 + 10 + 26 + Char - 97);
        } else if(Flags & Z_FONT_FLAG_ALPHA_U) {
            // fallback to uppercase
            frame = (char)(1 + 10 + Char - 97);
        }
    }

    z_sprite_blit(Sprite, X, Y, u8(frame));

    return i16(X + CharWidth + 1);
}

void z_font_text(const char* Text, int16_t X, int16_t Y, uint8_t Font)
{
    uint8_t flags = g_fonts[Font].flags;
    ZSprite* sprite = &g_fonts[Font].sprites;
    int8_t charWidth = z_sprite_getWidth(sprite);

    for(char c = *Text; c != '\0'; c = *++Text) {
        X = drawChar(c, X, Y, flags, sprite, charWidth);
    }
}

void z_font_textp(uint8_t StringId, int16_t X, int16_t Y, uint8_t Font)
{
    uint8_t flags = g_fonts[Font].flags;
    ZSprite* sprite = &g_fonts[Font].sprites;
    int8_t charWidth = z_sprite_getWidth(sprite);
    const char* s = z_strings[StringId];

    for(char c = Z_PGM_READ_UINT8(s); c != '\0'; c = Z_PGM_READ_UINT8(++s)) {
        X = drawChar(c, X, Y, flags, sprite, charWidth);
    }
}

void z_font_textCenterp(uint8_t StringId, int16_t X, int16_t Y, uint8_t Font)
{
    int8_t charWidth = z_sprite_getWidth(&g_fonts[Font].sprites);
    const char* s = z_strings[StringId];
    int8_t w = 0;

    for(char c = Z_PGM_READ_UINT8(s); c != '\0'; c = Z_PGM_READ_UINT8(++s)) {
        w = i8(w + charWidth + 1);
    }

    z_font_textp(StringId, i16(X - w / 2), Y, Font);
}

void z_font_textWrapp(uint8_t StringId, int16_t X, int16_t Y, uint8_t Font)
{
    uint8_t flags = g_fonts[Font].flags;
    ZSprite* sprite = &g_fonts[Font].sprites;
    int8_t charWidth = z_sprite_getWidth(sprite);
    int8_t charHeight = z_sprite_getHeight(sprite);
    const char* s = z_strings[StringId];
    int16_t lineWidth = 0;
    int16_t x = X;

    for(char c = Z_PGM_READ_UINT8(s); c != '\0'; c = Z_PGM_READ_UINT8(++s)) {
        if(c == ' ') {
            x = i16(x + charWidth + 1);
            lineWidth = i16(lineWidth + charWidth + 1);
            continue;
        }

        char c2 = c;
        const char* s2 = s;
        int16_t wordWidth = 0;

        for( ; c2 != ' ' && c2 != '\0'; c2 = Z_PGM_READ_UINT8(++s2)) {
            wordWidth = i16(wordWidth + charWidth + 1);
        }

        if(lineWidth > 0 && X + lineWidth + wordWidth >= Z_WIDTH - X) {
            x = X;
            Y = i16(Y + charHeight + 1);
            lineWidth = wordWidth;
        } else {
            lineWidth = i16(lineWidth + wordWidth);
        }

        c2 = c;
        s2 = s;

        for( ; c2 != ' ' && c2 != '\0'; c2 = Z_PGM_READ_UINT8(++s2)) {
            x = drawChar(c2, x, Y, flags, sprite, charWidth);
        }

        s = s2 - 1;
    }
}

void z_font_int(int16_t Number, int16_t X, int16_t Y, uint8_t Font)
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

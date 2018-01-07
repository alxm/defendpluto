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

Z_EXTERN_C_START

typedef enum {
    Z_FONT_FACE_INVALID = -1,
    Z_FONT_FACE_LCD,
    Z_FONT_FACE_RED,
    Z_FONT_FACE_REDO,
    Z_FONT_FACE_YELLOWO,
    Z_FONT_FACE_NUM
} ZFontFace;

typedef enum {
    Z_FONT_ALIGN_INVALID = -1,
    Z_FONT_ALIGN_L,
    Z_FONT_ALIGN_C,
    Z_FONT_ALIGN_R,
    Z_FONT_ALIGN_NUM
} ZFontAlign;

extern void z_font_setup(void);

extern void z_font_text(uint8_t StringId, int16_t X, int16_t Y, uint8_t Font, uint8_t Align);
extern void z_font_textWrap(uint8_t StringId, int16_t X, int16_t Y, uint8_t Font);
extern void z_font_int(int16_t Number, int16_t X, int16_t Y, uint8_t Font, uint8_t Align);

Z_EXTERN_C_END

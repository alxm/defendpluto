/*
    Copyright 2017-2018 Alex Margarit <alex@alxm.org>
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

#pragma once

#include "util_str.h"

Z_EXTERN_C_START

typedef enum Z_ENUM_PACK {
    Z_FONT_FACE_INVALID = -1,
    Z_FONT_FACE_LCD,
    Z_FONT_FACE_RED,
    Z_FONT_FACE_REDO,
    Z_FONT_FACE_YELLOW,
    Z_FONT_FACE_YELLOWO,
    Z_FONT_FACE_NUM
} ZFontId;

typedef enum Z_ENUM_PACK {
    Z_FONT_ALIGN_INVALID = -1,
    Z_FONT_ALIGN_L,
    Z_FONT_ALIGN_C,
    Z_FONT_ALIGN_R,
    Z_FONT_ALIGN_NUM
} ZFontAlign;

extern void z_font_setup(void);

extern void z_font_text(ZStringId StringId, int16_t X, int16_t Y, ZFontId Font, ZFontAlign Align);
extern void z_font_textWrap(ZStringId StringId, int16_t X, int16_t Y, ZFontId Font);
extern void z_font_int(int16_t Number, int16_t X, int16_t Y, ZFontId Font, ZFontAlign Align);

Z_EXTERN_C_END

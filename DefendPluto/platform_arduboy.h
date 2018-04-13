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

#include <Arduino.h>

#ifdef __cplusplus
#include <Arduboy2.h>
#include <Sprites.h>
#endif

Z_EXTERN_C_START

#undef Z_ENUM_PACK
#define Z_ENUM_PACK __attribute__((packed))

typedef uint8_t ZPixel;
typedef uint8_t ZPoolObjOffset;

#include "util_graphics.h"

#define Z_SCREEN_W 128
#define Z_SCREEN_H 64

#define z_sprite_load(Index, Id)                      \
    z_platform__loadSprite(Index,                     \
                           z_data_gfx_##Id##_buffer,  \
                           z_data_gfx_##Id##_frames);

extern void z_platform__loadSprite(ZSpriteId Sprite, const uint8_t* Buffer, uint8_t NumFrames);

static inline char z_pgm_readChar(const char* Address)
{
    return pgm_read_byte(Address);
}

static inline uint8_t z_pgm_readU8(const uint8_t* Address)
{
    return pgm_read_byte(Address);
}

static inline int16_t z_pgm_readI16(const int16_t* Address)
{
    return pgm_read_word(Address);
}

static inline uint16_t z_pgm_readU16(const uint16_t* Address)
{
    return pgm_read_word(Address);
}

Z_EXTERN_C_END

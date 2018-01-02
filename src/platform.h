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

#ifdef ARDUINO_AVR_LEONARDO
    #define Z_PLATFORM_ARDUBOY 1
#endif

#ifdef ARDUINO_SAMD_ZERO
    #define Z_PLATFORM_GAMEBUINOMETA 1
#endif

#define Z_DEBUG_GENERATE_LUT 0
#define Z_DEBUG_STATS 0
#define Z_DEBUG_INVINCIBLE 0
#define Z_DEBUG_INFINITE_ENERGY 0

#ifdef __cplusplus
#define Z_EXTERN_C_START extern "C" {
#define Z_EXTERN_C_END }
#else
#define Z_EXTERN_C_START
#define Z_EXTERN_C_END
#endif

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef ARDUINO
    #include <Arduino.h>
    #if Z_PLATFORM_ARDUBOY
        #define Z_PGM_READ_UINT8(Value) pgm_read_byte(Value)
        #define Z_PGM_READ_UINT16(Value) pgm_read_word(Value)
    #endif
    typedef uint8_t ZPoolOffset;
#else
    #define PROGMEM
    typedef size_t ZPoolOffset;
#endif

#ifndef Z_PGM_READ_UINT8
    #define Z_PGM_READ_UINT8(Value) (*(Value))
    #define Z_PGM_READ_UINT16(Value) (*(Value))
#endif

#define i8(X) (int8_t)(X)
#define i16(X) (int16_t)(X)
#define u2(X) ((uint8_t)(X) & 0x3u)
#define u4(X) ((uint8_t)(X) & 0xfu)
#define u8(X) (uint8_t)(X)
#define u16(X) (uint16_t)(X)
#define zf(X) (ZFix)(X)
#define zpo(X) (ZPoolOffset)(X)
#define bf(X, Bits) u8((X) & ((1 << (Bits)) - 1))

Z_EXTERN_C_START

typedef enum {
    Z_PALETTE_INVALID = -1,
    Z_PALETTE_ARDUBOY,
    Z_PALETTE_DEFAULT,
    Z_PALETTE_X1,
    Z_PALETTE_X2,
    Z_PALETTE_NUM
} ZPalette;

#define Z_UNUSED(X) (X = X)
#define Z_ARRAY_LEN(A) (sizeof(A) / sizeof(A[0]))

#if Z_PLATFORM_ARDUBOY
    #define z_sprite_load(Index, Id)                      \
        z_platform__loadSprite(Index,                     \
                               z_data_gfx_##Id##_buffer,  \
                               z_data_gfx_##Id##_frames);

    extern void z_platform__loadSprite(uint8_t Sprite, const uint8_t* Buffer, uint8_t NumFrames);
#elif Z_PLATFORM_GAMEBUINOMETA
    #define z_sprite_load(Index, Id)                      \
        z_platform__loadSprite(Index,                     \
                               z_data_gfx_##Id##_buffer,  \
                               z_data_gfx_##Id##_frames);

    extern void z_platform__loadSprite(uint8_t Sprite, const uint16_t* Buffer, uint8_t NumFrames);
#else
    #define z_sprite_load(Index, Id)                      \
        z_platform__loadSprite(Index, "gfx/" #Id ".png");

    extern void z_platform__loadSprite(uint8_t Sprite, const char* Path);
#endif

extern void z_platform_setup(void);
extern void z_platform_tick(void);
extern void z_platform_draw(void);

Z_EXTERN_C_END

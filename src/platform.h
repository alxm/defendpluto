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

#define Z_DEBUG_GENERATE_LUT 0
#define Z_DEBUG_STATS 0
#define Z_DEBUG_INVINCIBLE 1

#ifdef __cplusplus
#define Z_EXTERN_C_START extern "C" {
#define Z_EXTERN_C_END }
#else
#define Z_EXTERN_C_START
#define Z_EXTERN_C_END
#endif

#ifdef ARDUINO
    #include <stdlib.h>
    #include <stdbool.h>
    #include <stdint.h>
    #include <Arduino.h>
    #ifdef __cplusplus
        #include <Arduboy2.h>
    #endif
    #define Z_PGM_READ_UINT8(Value) pgm_read_byte(&Value)
    #define Z_PGM_READ_UINT16(Value) pgm_read_word(&Value)
#else
    #include <a2x.h>
    #define PROGMEM
    #define Z_PGM_READ_UINT8(Value) (Value)
    #define Z_PGM_READ_UINT16(Value) (Value)
#endif

typedef int16_t ZFix;

#define i8(X) (int8_t)(X)
#define i16(X) (int16_t)(X)
#define u4(X) ((uint8_t)(X) & 0xfu)
#define u8(X) (uint8_t)(X)
#define u16(X) (uint16_t)(X)
#define zf(X) (ZFix)(X)

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

#ifdef ARDUINO
    typedef uint8_t ZButton;
    typedef struct {
        const uint8_t* image;
        const uint8_t* mask;
        uint8_t numFrames;
    } ZSprite;

    #define z_sprite_load(Sprite, Id)                    \
        z_platform__loadSprite(Sprite,                   \
                               z_data_gfx_##Id##_buffer, \
                               z_data_gfx_##Id##_mask,   \
                               z_data_gfx_##Id##_frames);

    extern void z_platform__loadSprite(ZSprite* Sprite, const uint8_t* Image, const uint8_t* Mask, uint8_t NumFrames);
#else
    typedef AInputButton* ZButton;
    typedef struct {
        ASpriteFrames* frames[Z_PALETTE_NUM];
        uint8_t numFrames;
    } ZSprite;

    #define z_sprite_load(Sprite, Id)                      \
        z_platform__loadSprite(Sprite, "gfx/" #Id ".png");

    extern void z_platform__loadSprite(ZSprite* Sprite, const char* Path);
#endif

extern void z_platform_setup(void);
extern void z_platform_tick(void);
extern void z_platform_draw(void);

Z_EXTERN_C_END

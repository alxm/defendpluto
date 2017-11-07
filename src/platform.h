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

#ifdef __cplusplus
extern "C" {
#endif

#define Z_UNUSED(X) (X = X)

#define Z_WIDTH 128
#define Z_HEIGHT 64
#define Z_FPS 30

typedef enum {
    Z_PALETTE_INVALID = -1,
    Z_PALETTE_ARDUBOY,
    Z_PALETTE_DEFAULT,
    Z_PALETTE_NUM
} ZPalette;

typedef enum {
    Z_COLOR_INVALID = -1,
    Z_COLOR_BLUE,
    Z_COLOR_LIGHTBLUE,
    Z_COLOR_LIGHTBLUE2,
    Z_COLOR_RED,
    Z_COLOR_YELLOW,
    Z_COLOR_NUM
} ZColor;

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

typedef struct {
    ZButton up;
    ZButton down;
    ZButton left;
    ZButton right;
    ZButton a;
    ZButton b;
} ZControls;

extern ZControls z_controls;

extern void z_platform_setup(void);
extern void z_platform_tick(void);

extern uint16_t z_fps_getCounter(void);
extern bool z_fps_isNthFrame(uint8_t N);

extern bool z_button_pressed(ZButton Button);

extern void z_draw_fill(uint8_t Color);
extern void z_draw_rectangle(int8_t X, int8_t Y, int8_t W, int8_t H, uint8_t Color);
extern void z_draw_pixel(int8_t X, int8_t Y, uint8_t Color);
extern void z_draw_circle(int8_t X, int8_t Y, uint8_t Radius, uint8_t Color);

extern void z_sprite_blit(ZSprite* Sprite, int8_t X, int8_t Y, uint8_t Frame);
extern void z_sprite_blitCentered(ZSprite* Sprite, int8_t X, int8_t Y, uint8_t Frame);
extern int8_t z_sprite_getWidth(ZSprite* Sprite);
extern int8_t z_sprite_getHeight(ZSprite* Sprite);

#ifdef __cplusplus
}
#endif

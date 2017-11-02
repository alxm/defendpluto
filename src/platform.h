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
    Z_COLOR_BLUE,
    Z_COLOR_YELLOW,
    Z_COLOR_RED,
    Z_COLOR_LIGHTBLUE,
    Z_COLOR_NUM
} ZColors;

#ifdef ARDUINO
    typedef uint8_t ZButton;
    typedef const uint8_t* ZSprite;
#else
    typedef AInputButton* ZButton;
    typedef ASprite* ZSprite;
#endif

typedef struct {
    ZButton up;
    ZButton down;
    ZButton left;
    ZButton right;
    ZButton a;
    ZButton b;
} ZControls;

typedef struct {
    ZSprite enemy[3];
    ZSprite player[3];
} ZGfx;

extern ZControls z_controls;
extern ZGfx z_gfx;

extern void z_platform_setup(void);

extern uint16_t z_fps_getCounter(void);
extern bool z_fps_isNthFrame(uint8_t N);

extern bool z_button_pressed(ZButton Button);

extern void z_draw_fill(uint8_t Color);
extern void z_draw_rectangle(int8_t X, int8_t Y, int8_t W, int8_t H, uint8_t Color);
extern void z_draw_pixel(int8_t X, int8_t Y, uint8_t Color);
extern void z_draw_circle(int8_t X, int8_t Y, uint8_t Radius, uint8_t Color);

extern void z_sprite_blit(ZSprite Sprite, int8_t X, int8_t Y);
extern int8_t z_sprite_getWidth(ZSprite Sprite);
extern int8_t z_sprite_getHeight(ZSprite Sprite);

static inline int8_t z_random_int8(int8_t Max)
{
    return (int8_t)(rand() % Max);
}

static inline uint8_t z_random_uint8(uint8_t Max)
{
    return (uint8_t)(rand() % Max);
}

static inline int16_t z_random_int16(int16_t Max)
{
    return (int16_t)(rand() % Max);
}

static inline uint16_t z_random_uint16(uint16_t Max)
{
    return (uint16_t)(rand() % Max);
}

#ifdef __cplusplus
}
#endif

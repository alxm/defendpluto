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

Z_EXTERN_C_START

typedef enum {
    Z_COLOR_INVALID = -1,
    Z_COLOR_BLUE,
    Z_COLOR_LIGHTBLUE,
    Z_COLOR_LIGHTBLUE2,
    Z_COLOR_RED,
    Z_COLOR_YELLOW,
    Z_COLOR_NUM
} ZColor;

typedef enum {
    Z_BIT_RESTING = 0,
    Z_BIT_FORWARD = (1 << 0),
    Z_BIT_BACK    = (1 << 1),
    Z_BIT_LEFT    = (1 << 2),
    Z_BIT_RIGHT   = (1 << 3),
    Z_BIT_MAX_VAL = (Z_BIT_RIGHT | Z_BIT_BACK)
} ZFrameBits;

typedef struct {
    ZSprite player[Z_BIT_MAX_VAL + 1];
    ZSprite bullets;
    ZSprite hearts;
    ZSprite shield;
    ZSprite title;
} ZGraphics;

extern ZGraphics z_graphics;

extern void z_graphics_setup(void);

extern void z_sprite_blit(ZSprite* Sprite, int8_t X, int8_t Y, uint8_t Frame);
extern void z_sprite_blitCentered(ZSprite* Sprite, int8_t X, int8_t Y, uint8_t Frame);
extern int8_t z_sprite_getWidth(ZSprite* Sprite);
extern int8_t z_sprite_getHeight(ZSprite* Sprite);

extern void z_draw_fill(uint8_t Color);
extern void z_draw_rectangle(int8_t X, int8_t Y, int8_t W, int8_t H, uint8_t Color);
extern void z_draw_pixel(int8_t X, int8_t Y, uint8_t Color);
extern void z_draw_circle(int8_t X, int8_t Y, uint8_t Radius, uint8_t Color);

Z_EXTERN_C_END

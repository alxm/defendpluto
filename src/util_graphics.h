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
    Z_COLOR_INVALID = -1,
    Z_COLOR_ALXM_BG,
    Z_COLOR_BLUE,
    Z_COLOR_PURPLE,
    Z_COLOR_GRAY,
    Z_COLOR_RED,
    Z_COLOR_YELLOW,
    Z_COLOR_ALXM_Y1,
    Z_COLOR_ALXM_Y2,
    Z_COLOR_ALXM_G1,
    Z_COLOR_ALXM_G2,
    Z_COLOR_ALXM_B1,
    Z_COLOR_ALXM_B2,
    Z_COLOR_ALXM_R1,
    Z_COLOR_ALXM_R2,
    Z_COLOR_NUM
} ZColor;

typedef enum {
    Z_BIT_RESTING = 0,
    Z_BIT_FORWARD = (1 << 0),
    Z_BIT_BACK    = (1 << 1),
    Z_BIT_LEFT    = (1 << 2),
    Z_BIT_RIGHT   = (1 << 3),
} ZFrameBits;

typedef enum {
    // Enemies
    Z_SPRITE_ASTEROID,
    Z_SPRITE_ENEMY00,
    Z_SPRITE_ENEMY01,
    Z_SPRITE_ENEMY02,

    // Fonts
    Z_SPRITE_FONT_NUM,
    Z_SPRITE_FONT_ALPHANUM,
    Z_SPRITE_FONT_ALPHANUM_OUTLINE,
    Z_SPRITE_FONT_ALPHANUM_OUTLINE_YELLOW,

    // Icons
    Z_SPRITE_CREDS,
    Z_SPRITE_ENERGY,
    Z_SPRITE_HEARTS,
    Z_SPRITE_SHIELD,

    // Player ship
    Z_SPRITE_PLAYER,

    // Misc
    Z_SPRITE_ALXM,
    Z_SPRITE_BULLETS,
    Z_SPRITE_DEFENDPLUTO,
    Z_SPRITE_TITLE,
    Z_SPRITE_NUM
} ZSpriteId;

extern void z_graphics_setup(void);

extern void z_sprite_blit(uint8_t Sprite, int16_t X, int16_t Y, uint8_t Frame);
extern void z_sprite_blitCentered(uint8_t Sprite, int16_t X, int16_t Y, uint8_t Frame);
extern int16_t z_sprite_getWidth(uint8_t Sprite);
extern int16_t z_sprite_getHeight(uint8_t Sprite);
extern uint8_t z_sprite_getNumFrames(uint8_t Sprite);

extern void z_draw_fill(uint8_t Color);
extern void z_draw_rectangle(int16_t X, int16_t Y, int16_t W, int16_t H, uint8_t Color);
extern void z_draw_pixel(int16_t X, int16_t Y, uint8_t Color);
extern void z_draw_circle(int16_t X, int16_t Y, int16_t Radius, uint8_t Color);
extern void z_draw_hline(int16_t X1, int16_t X2, int16_t Y, uint8_t Color);
extern void z_draw_vline(int16_t X, int16_t Y1, int16_t Y2, uint8_t Color);

Z_EXTERN_C_END

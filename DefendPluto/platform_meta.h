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
#include <Gamebuino-Meta.h>
#endif

Z_EXTERN_C_START

typedef uint16_t ZPixel;

#include "util_graphics.h"
#include "util_sound.h"

#define Z_SCREEN_W 80
#define Z_SCREEN_H 64

#define z_sprite_load(Index, Id)                      \
    z_platform__loadSprite(Index,                     \
                           z_data_gfx_##Id##_buffer,  \
                           z_data_gfx_##Id##_frames);

#define z_sfx_load(Index, Id)                     \
    z_platform__loadSfx(Index,                    \
                        z_data_sfx_##Id##_buffer, \
                        z_data_sfx_##Id##_size);

extern void z_platform__loadSprite(ZSpriteId Sprite, const uint16_t* Buffer, uint8_t NumFrames);
extern void z_platform__loadSfx(ZSfxId Sfx, const uint8_t* Buffer, uint32_t Size);

extern void z_platform_meta_fillLights(ZColorId ColorId, int Intensity);
extern void z_platform_meta_drawLights(ZColorId ColorId, int Intensity, int16_t X, int16_t Y);

Z_EXTERN_C_END

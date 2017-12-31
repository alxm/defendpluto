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

#include "platform.h"
#include "util_graphics.h"
#include "util_screen.h"

#include "data_gfx_asteroid.h"
#include "data_gfx_bullets.h"
#include "data_gfx_enemy00.h"
#include "data_gfx_enemy01.h"
#include "data_gfx_enemy02.h"
#include "data_gfx_energy.h"
#include "data_gfx_font_num.h"
#include "data_gfx_font_alphanum.h"
#include "data_gfx_font_alphanum_outline.h"
#include "data_gfx_font_alphanum_outline_yellow.h"
#include "data_gfx_hearts.h"
#include "data_gfx_player.h"
#include "data_gfx_shield.h"
#include "data_gfx_title.h"
#include "data_gfx_title80.h"

void z_graphics_setup(void)
{
    z_sprite_load(Z_SPRITE_ASTEROID, asteroid);
    z_sprite_load(Z_SPRITE_BULLETS, bullets);
    z_sprite_load(Z_SPRITE_ENEMY00, enemy00);
    z_sprite_load(Z_SPRITE_ENEMY01, enemy01);
    z_sprite_load(Z_SPRITE_ENEMY02, enemy02);
    z_sprite_load(Z_SPRITE_ENERGY, energy);
    z_sprite_load(Z_SPRITE_FONT_NUM, font_num);
    z_sprite_load(Z_SPRITE_FONT_ALPHANUM, font_alphanum);
    z_sprite_load(Z_SPRITE_FONT_ALPHANUM_OUTLINE, font_alphanum_outline);
    z_sprite_load(Z_SPRITE_FONT_ALPHANUM_OUTLINE_YELLOW, font_alphanum_outline_yellow);
    z_sprite_load(Z_SPRITE_HEARTS, hearts);
    z_sprite_load(Z_SPRITE_PLAYER, player);
    z_sprite_load(Z_SPRITE_SHIELD, shield);

    #if Z_PLATFORM_ARDUBOY
        z_sprite_load(Z_SPRITE_TITLE, title);
    #else
        z_sprite_load(Z_SPRITE_TITLE, title80);
    #endif
}

void z_sprite_blitCentered(uint8_t Sprite, int16_t X, int16_t Y, uint8_t Frame)
{
    z_sprite_blit(Sprite,
                  i16(X - z_sprite_getWidth(Sprite) / 2),
                  i16(Y - z_sprite_getHeight(Sprite) / 2),
                  Frame);
}

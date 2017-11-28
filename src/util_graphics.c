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
#include "data_gfx_player.h"
#include "data_gfx_player_left.h"
#include "data_gfx_player_right.h"
#include "data_gfx_player_forward.h"
#include "data_gfx_player_forward_left.h"
#include "data_gfx_player_forward_right.h"
#include "data_gfx_player_back.h"
#include "data_gfx_player_back_left.h"
#include "data_gfx_player_back_right.h"
#include "data_gfx_bullets.h"
#include "data_gfx_energy.h"
#include "data_gfx_hearts.h"
#include "data_gfx_shield.h"
#include "data_gfx_title.h"

ZGraphics z_graphics;

void z_graphics_setup(void)
{
    z_sprite_load(&z_graphics.player[0][0], player_forward_left);
    z_sprite_load(&z_graphics.player[0][1], player_forward);
    z_sprite_load(&z_graphics.player[0][2], player_forward_right);
    z_sprite_load(&z_graphics.player[1][0], player_left);
    z_sprite_load(&z_graphics.player[1][1], player);
    z_sprite_load(&z_graphics.player[1][2], player_right);
    z_sprite_load(&z_graphics.player[2][0], player_back_left);
    z_sprite_load(&z_graphics.player[2][1], player_back);
    z_sprite_load(&z_graphics.player[2][2], player_back_right);
    z_sprite_load(&z_graphics.bullets, bullets);
    z_sprite_load(&z_graphics.energy, energy);
    z_sprite_load(&z_graphics.hearts, hearts);
    z_sprite_load(&z_graphics.shield, shield);
    z_sprite_load(&z_graphics.title, title);
}

void z_sprite_blitCentered(ZSprite* Sprite, int16_t X, int16_t Y, uint8_t Frame)
{
    z_sprite_blit(Sprite,
                  i16(X - z_sprite_getWidth(Sprite) / 2),
                  i16(Y - z_sprite_getHeight(Sprite) / 2),
                  Frame);
}

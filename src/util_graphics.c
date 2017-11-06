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

#include "platform.h"
#include "util_graphics.h"
#include "data_gfx_enemy00.h"
#include "data_gfx_enemy01.h"
#include "data_gfx_enemy02.h"
#include "data_gfx_player.h"
#include "data_gfx_player_left.h"
#include "data_gfx_player_right.h"
#include "data_gfx_player_forward.h"
#include "data_gfx_player_forward_left.h"
#include "data_gfx_player_forward_right.h"
#include "data_gfx_player_back.h"
#include "data_gfx_player_back_left.h"
#include "data_gfx_player_back_right.h"
#include "data_gfx_hearts.h"

ZGraphics z_graphics;

void z_graphics_setup(void)
{
    z_sprite_load(&z_graphics.enemy[0], enemy00);
    z_sprite_load(&z_graphics.enemy[1], enemy01);
    z_sprite_load(&z_graphics.enemy[2], enemy02);
    z_sprite_load(&z_graphics.player[Z_BIT_RESTING], player);
    z_sprite_load(&z_graphics.player[Z_BIT_LEFT], player_left);
    z_sprite_load(&z_graphics.player[Z_BIT_RIGHT], player_right);
    z_sprite_load(&z_graphics.player[Z_BIT_FORWARD], player_forward);
    z_sprite_load(&z_graphics.player[Z_BIT_FORWARD | Z_BIT_LEFT], player_forward_left);
    z_sprite_load(&z_graphics.player[Z_BIT_FORWARD | Z_BIT_RIGHT], player_forward_right);
    z_sprite_load(&z_graphics.player[Z_BIT_BACK], player_back);
    z_sprite_load(&z_graphics.player[Z_BIT_BACK | Z_BIT_LEFT], player_back_left);
    z_sprite_load(&z_graphics.player[Z_BIT_BACK | Z_BIT_RIGHT], player_back_right);
    z_sprite_load(&z_graphics.hearts, hearts);
}

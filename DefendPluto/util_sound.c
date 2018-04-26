/*
    Copyright 2018 Alex Margarit <alex@alxm.org>

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
#include "util_sound.h"

#include "generated/data_sfx_enemy_die.h"
#include "generated/data_sfx_enemy_hurt.h"
#include "generated/data_sfx_enemy_shoot.h"
#include "generated/data_sfx_level_complete.h"
#include "generated/data_sfx_level_lost.h"
#include "generated/data_sfx_level_won.h"
#include "generated/data_sfx_player_die.h"
#include "generated/data_sfx_player_hurt.h"
#include "generated/data_sfx_player_shoot.h"
#include "generated/data_sfx_pressed_a.h"
#include "generated/data_sfx_shield_deploy.h"
#include "generated/data_sfx_swipe_hide.h"
#include "generated/data_sfx_swipe_show.h"
#include "generated/data_sfx_title_screen.h"

void z_sound_setup(void)
{
    z_sfx_load(Z_SFX_ENEMY_DIE, enemy_die);
    z_sfx_load(Z_SFX_ENEMY_HURT, enemy_hurt);
    z_sfx_load(Z_SFX_ENEMY_SHOOT, enemy_shoot);
    z_sfx_load(Z_SFX_LEVEL_COMPLETE, level_complete);
    z_sfx_load(Z_SFX_LEVEL_LOST, level_lost);
    z_sfx_load(Z_SFX_LEVEL_WON, level_won);
    z_sfx_load(Z_SFX_PLAYER_DIE, player_die);
    z_sfx_load(Z_SFX_PLAYER_HURT, player_hurt);
    z_sfx_load(Z_SFX_PLAYER_SHOOT, player_shoot);
    z_sfx_load(Z_SFX_PRESSED_A, pressed_a);
    z_sfx_load(Z_SFX_SHIELD_DEPLOY, shield_deploy);
    z_sfx_load(Z_SFX_SWIPE_HIDE, swipe_hide);
    z_sfx_load(Z_SFX_SWIPE_SHOW, swipe_show);
    z_sfx_load(Z_SFX_TITLE_SCREEN, title_screen);
}

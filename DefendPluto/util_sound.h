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

#pragma once

Z_EXTERN_C_START

typedef enum Z_ENUM_PACK {
    Z_SFX_INVALID = -1,
    Z_SFX_ENEMY_DIE,
    Z_SFX_ENEMY_HURT,
    Z_SFX_ENEMY_SHOOT,
    Z_SFX_LEVEL_COMPLETE,
    Z_SFX_LEVEL_LOST,
    Z_SFX_LEVEL_WON,
    Z_SFX_PLAYER_DIE,
    Z_SFX_PLAYER_HURT,
    Z_SFX_PLAYER_SHOOT,
    Z_SFX_PRESSED_A,
    Z_SFX_SHIELD_DEPLOY,
    Z_SFX_SWIPE_HIDE,
    Z_SFX_SWIPE_SHOW,
    Z_SFX_TITLE_SCREEN,
    Z_SFX_NUM
} ZSfxId;

extern void z_sound_setup(void);

extern void z_sfx_play(ZSfxId Sfx);

Z_EXTERN_C_END

/*
    Copyright 2018 Alex Margarit <alex@alxm.org>
    This file is part of Defend Pluto, a video game.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "platform.h"
#include "state_new.h"

#include "obj_player.h"
#include "util_hud.h"
#include "util_light.h"
#include "util_pool.h"
#include "util_screen.h"
#include "util_vm.h"

void z_state_new_init(void)
{
    z_hud_reset();
    z_light_reset();
    z_screen_reset();
    z_vm_reset();

    z_player_init();

    z_pool_reset(Z_POOL_BULLETE);
    z_pool_reset(Z_POOL_BULLETP);
    z_pool_reset(Z_POOL_CIRCLE);
    z_pool_reset(Z_POOL_ENEMY);
    z_pool_reset(Z_POOL_PARTICLE);

    z_state_set(Z_STATE_PLAY, true);
}

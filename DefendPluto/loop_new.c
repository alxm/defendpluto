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
#include "loop_new.h"

#include "obj_player.h"
#include "util_hud.h"
#include "util_pool.h"
#include "util_screen.h"
#include "util_vm.h"

void z_loop_new_init(void)
{
    z_screen_reset();
    z_pool_reset();
    z_vm_reset();
    z_player_init();
    z_hud_reset();

    z_loop_setStateEx(Z_STATE_PLAY, Z_SWIPE_INVALID, Z_SWIPE_SHOW);
}

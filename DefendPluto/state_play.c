/*
    Copyright 2017-2018 Alex Margarit <alex@alxm.org>
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
#include "state_play.h"

#include "obj_bullete.h"
#include "obj_bulletp.h"
#include "obj_circle.h"
#include "obj_enemy.h"
#include "obj_particle.h"
#include "obj_player.h"
#include "obj_star.h"
#include "util_hud.h"
#include "util_input.h"
#include "util_light.h"
#include "util_pool.h"
#include "util_screen.h"
#include "util_vm.h"

void z_state_play_tick(void)
{
    if(!z_state_changed()) {
        z_vm_tick();
    }

    z_player_tick();
    z_pool_tick(Z_POOL_ENEMY, z_enemy_tick, NULL); // check player collision
    z_pool_tick(Z_POOL_BULLETE, z_bullete_tick, NULL); // check player collision
    z_pool_tick(Z_POOL_BULLETP, z_bulletp_tick, NULL); // check enemy collision
    z_pool_tick(Z_POOL_STAR, z_star_tick, NULL);
    z_pool_tick(Z_POOL_CIRCLE, z_circle_tick, NULL);
    z_pool_tick(Z_POOL_PARTICLE, z_particle_tick, NULL);
    z_hud_tick();

    if(z_state_changed()) {
        return;
    } else if(z_player_getHealth() < 0) {
        z_state_set(Z_STATE_DIED, false);
    } else if(z_button_pressedOnce(Z_BUTTON_MENU)) {
        z_state_set(Z_STATE_PAUSE, false);
    }
}

void z_state_play_draw(void)
{
    z_draw_fill(Z_COLOR_BLUE);
    z_pool_draw(Z_POOL_STAR, z_star_draw);
    z_pool_draw(Z_POOL_BULLETE, z_bullete_draw);
    z_pool_draw(Z_POOL_BULLETP, z_bulletp_draw);
    z_pool_draw(Z_POOL_ENEMY, z_enemy_draw);
    z_pool_draw(Z_POOL_CIRCLE, z_circle_draw);
    z_pool_draw(Z_POOL_PARTICLE, z_particle_draw);
    z_player_draw();
    z_hud_draw();
}

void z_state_play_free(void)
{
    z_light_setBackground(Z_COLOR_INVALID);
}

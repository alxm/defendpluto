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

#include "platform.h"
#include "loop.h"
#include "util_fix.h"
#include "util_effects.h"
#include "util_fps.h"
#include "util_graphics.h"
#include "util_hud.h"
#include "util_input.h"
#include "util_pool.h"
#include "util_random.h"
#include "util_screen.h"
#include "util_vm.h"
#include "obj_bullete.h"
#include "obj_bulletp.h"
#include "obj_circle.h"
#include "obj_enemy.h"
#include "obj_particle.h"
#include "obj_player.h"
#include "obj_star.h"

void z_loop_died_tick(void)
{
    z_screen_tick();
    z_vm_tick();
    z_hud_tick();
    z_pool_tick(Z_POOL_STAR, z_star_tick, NULL);
    z_pool_tick(Z_POOL_BULLETE, z_bullete_tick, NULL);
    z_pool_tick(Z_POOL_BULLETP, z_bulletp_tick, NULL);
    z_pool_tick(Z_POOL_ENEMY, z_enemy_tick, NULL);
    z_pool_tick(Z_POOL_CIRCLE, z_circle_tick, NULL);
    z_pool_tick(Z_POOL_PARTICLE, z_particle_tick, NULL);

    Z_EVERY_DS(10) {
        if(z_player.health-- < -2) {
            z_button_release(Z_BUTTON_A);
            z_loop_setState(Z_STATE_DOORS_CLOSE);
        }
    }

    Z_EVERY_DS(1) {
        z_screen_shake(2);

        if(z_random_uint8(4) == 0) {
            z_effect_circles(
                zf(z_player.x + Z_FIX_ONE * (-1 + z_random_int8(3))),
                zf(z_player.y + Z_FIX_ONE * (-1 + z_random_int8(3))));
        } else {
            z_effect_particles(z_player.x, z_player.y, 1);
        }
    }
}

void z_loop_died_draw(void)
{
    z_draw_fill(Z_COLOR_BLUE);
    z_pool_draw(Z_POOL_STAR, z_star_draw);
    z_pool_draw(Z_POOL_BULLETE, z_bullete_draw);
    z_pool_draw(Z_POOL_BULLETP, z_bulletp_draw);
    z_pool_draw(Z_POOL_ENEMY, z_enemy_draw);
    z_pool_draw(Z_POOL_CIRCLE, z_circle_draw);
    z_pool_draw(Z_POOL_PARTICLE, z_particle_draw);
    z_hud_draw();
}

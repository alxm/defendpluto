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
#include "util_enemy.h"
#include "util_fix.h"
#include "util_font.h"
#include "util_graphics.h"
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

void loop_setup(void)
{
    z_platform_setup();

    z_enemy_setup();
    z_font_setup();
    z_graphics_setup();
    z_pool_setup();
    z_vm_setup();

    z_player_init(Z_WIDTH / 2, Z_HEIGHT * 2 / 3);
}

static void loop_reset(void)
{
    z_pool_reset();
    z_screen_reset();
    z_vm_reset();

    z_player_init(Z_WIDTH / 2, Z_HEIGHT * 2 / 3);
}

void loop_tick(void)
{
    z_platform_tick();

    z_vm_tick();
    z_player_tick();
    z_pool_tick(Z_POOL_STAR, z_star_tick);
    z_pool_tick(Z_POOL_BULLETE, z_bullete_tick);
    z_pool_tick(Z_POOL_BULLETP, z_bulletp_tick);
    z_pool_tick(Z_POOL_ENEMY, z_enemy_tick);
    z_pool_tick(Z_POOL_CIRCLE, z_circle_tick);
    z_pool_tick(Z_POOL_PARTICLE, z_particle_tick);
    z_screen_tick();

    if(z_random_int8(2 * Z_HEIGHT / Z_STAR_POOL_NUM) == 0) {
        ZStar* star = z_pool_alloc(Z_POOL_STAR);

        if(star != NULL) {
            z_star_init(star);
        }
    }

    if(z_player.health < 0) {
        loop_reset();
    }
}

void loop_draw(void)
{
    z_draw_fill(Z_COLOR_BLUE);
    z_pool_draw(Z_POOL_STAR, z_star_draw);
    z_pool_draw(Z_POOL_BULLETE, z_bullete_draw);
    z_pool_draw(Z_POOL_BULLETP, z_bulletp_draw);
    z_pool_draw(Z_POOL_ENEMY, z_enemy_draw);
    z_pool_draw(Z_POOL_CIRCLE, z_circle_draw);
    z_pool_draw(Z_POOL_PARTICLE, z_particle_draw);
    z_player_draw();

    z_platform_draw();
}

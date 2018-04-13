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
#include "obj_star.h"

#include "obj_player.h"
#include "util_screen.h"

#define Z_STAR_BORDER_RATIO (8)
#define Z_STAR_SPEED_MIN    (Z_FIX_ONE / 8)
#define Z_STAR_SPEED_MAX    (Z_FIX_ONE - Z_FIX_ONE / 8)
#define Z_STAR_MULT_MIN     (4)
#define Z_STAR_MULT_MAX     (8)

static void z_star_init(ZStar* Star, int16_t Y)
{
    Star->x = z_fix_itofix(
        i16(Z_SCREEN_W / Z_STAR_BORDER_RATIO
                + z_random_int8(
                    Z_SCREEN_W - 2 * Z_SCREEN_W / Z_STAR_BORDER_RATIO)));

    Star->y = Y;
    Star->superSpeed = z_random_uint8(Z_POOL_NUM_STAR / 2) == 0;

    if(Star->superSpeed) {
        Star->speed = u7(Z_STAR_MULT_MIN
                + z_random_uint8(Z_STAR_MULT_MAX - Z_STAR_MULT_MIN));
    } else {
        Star->speed = u7(Z_STAR_SPEED_MIN
                + z_random_int16(Z_STAR_SPEED_MAX - Z_STAR_SPEED_MIN));
    }
}

void z_star_setup(void)
{
    for(int8_t i = Z_POOL_NUM_STAR; i--; ) {
        ZStar* star = z_pool_alloc(Z_POOL_STAR);
        z_star_init(star, z_fix_itofix(z_random_int16(Z_SCREEN_H)));
    }
}

bool z_star_tick(ZPoolObjHeader* Star, void* Context)
{
    Z_UNUSED(Context);

    ZStar* star = (ZStar*)Star;

    if(star->superSpeed) {
        star->y = zf(star->y + star->speed * Z_FIX_ONE);
    } else {
        star->y = zf(star->y + star->speed);
    }

    if(z_fix_fixtoi(star->y) >= Z_SCREEN_H) {
        z_star_init(star, 0);
    }

    return true;
}

void z_star_draw(ZPoolObjHeader* Star)
{
    ZStar* star = (ZStar*)Star;

    int16_t x = i16(z_fix_fixtoi(star->x) + z_screen_getXShake());
    int16_t y = i16(z_fix_fixtoi(star->y) + z_screen_getYShake());
    int16_t centerOffset = i16(z_fix_fixtoi(z_player.x) - Z_SCREEN_W / 2);
    uint8_t avgSpeed = (Z_STAR_SPEED_MIN + Z_STAR_SPEED_MAX) / 2;

    x = i16(x
        - (Z_SCREEN_W / Z_STAR_BORDER_RATIO * star->speed / Z_STAR_SPEED_MAX)
            * centerOffset / (Z_SCREEN_W / 2));

    if(star->superSpeed) {
        z_draw_rectangle(x, i16(y - 1), 1, 3, Z_COLOR_GRAY);
    } else {
        z_draw_pixel(x, y, Z_COLOR_PURPLE + (star->speed >= avgSpeed));
    }
}

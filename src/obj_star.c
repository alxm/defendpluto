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
#include "util_fix.h"
#include "util_graphics.h"
#include "util_pool.h"
#include "util_random.h"
#include "util_screen.h"
#include "obj_player.h"
#include "obj_star.h"

#define Z_STARS_BORDER   16
#define Z_STAR_MIN_SPEED (Z_FIX_ONE / 8)
#define Z_STAR_AVG_SPEED (Z_FIX_ONE / 2)
#define Z_STAR_RND_SPEED ((Z_STAR_AVG_SPEED - Z_STAR_MIN_SPEED) * 2)
#define Z_STAR_MAX_SPEED (Z_STAR_MIN_SPEED + Z_STAR_RND_SPEED)

void z_star_init(ZStar* Star)
{
    Star->x = z_fix_itofix(
        i8(Z_STARS_BORDER + z_random_int8(Z_WIDTH - 2 * Z_STARS_BORDER)));
    Star->y = 0;
    Star->speed = u8(Z_STAR_MIN_SPEED + z_random_int16(Z_STAR_RND_SPEED));
}

bool z_star_tick(ZPoolObject* Star)
{
    ZStar* star = (ZStar*)Star;

    star->y = zf(star->y + star->speed);

    return z_fix_fixtoi(star->y) < Z_HEIGHT;
}

void z_star_draw(ZPoolObject* Star)
{
    ZStar* star = (ZStar*)Star;

    int8_t x = i8(z_fix_fixtoi(star->x) + z_screen_getXShake());
    int8_t y = i8(z_fix_fixtoi(star->y) + z_screen_getYShake());

    int8_t centerOffset = i8(z_fix_fixtoi(z_player.x) - Z_WIDTH / 2);
    x = i8(x - (Z_STARS_BORDER * star->speed / Z_STAR_MAX_SPEED)
                    * centerOffset / (Z_WIDTH / 2));

    z_draw_pixel(x, y, Z_COLOR_LIGHTBLUE + (star->speed >= Z_STAR_AVG_SPEED));
}

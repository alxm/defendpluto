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
#include "util_fix.h"
#include "util_pool.h"
#include "util_screen.h"
#include "obj_player.h"
#include "obj_star.h"

#define Z_STARS_BORDER 16

void z_star_init(ZStar* Star)
{
    Star->x = z_fix_itofix(
        (int8_t)(Z_STARS_BORDER + z_random_int8(Z_WIDTH - 2 * Z_STARS_BORDER)));
    Star->y = 0;
    Star->speed = (ZFix)(32 + z_random_int16(192));
}

bool z_star_tick(ZPoolObject* Star)
{
    ZStar* star = (ZStar*)Star;

    star->y = (ZFix)(star->y + star->speed);

    return z_fix_fixtoi(star->y) < Z_HEIGHT;
}

void z_star_draw(ZPoolObject* Star)
{
    ZStar* star = (ZStar*)Star;

    int8_t x = (int8_t)(z_fix_fixtoi(star->x) + z_screen_xShake);
    int8_t y = (int8_t)(z_fix_fixtoi(star->y) + z_screen_yShake);

    int8_t centerOffset = (int8_t)(z_fix_fixtoi(z_player.x) - Z_WIDTH / 2);
    x = (int8_t)(x - Z_STARS_BORDER * centerOffset / (Z_WIDTH / 2));

    z_draw_pixel(x, y, Z_COLOR_LIGHTBLUE);
}

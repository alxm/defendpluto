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

#include "shared.h"
#include "util_fix.h"
#include "util_pool.h"
#include "obj_star.h"

void z_star_init(ZStar* Star)
{
    Star->x = (fix)(rand() % S_WIDTH);
    Star->y = 0;
    Star->speed = (fix)(FIX_ONE / STAR_SPEED_DIV / 2
                            + (rand() % (FIX_ONE / STAR_SPEED_DIV)));
}

bool z_star_tick(ZStar* Star)
{
    Star->y = (fix)(Star->y + Star->speed);

    return Star->y >> FIX_PRECISION_BITS >= S_HEIGHT;
}

void z_star_draw(ZStar* Star)
{
    s_draw_pixel(Star->x, Star->y >> FIX_PRECISION_BITS, true);
}

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
#include "util_pool.h"
#include "obj_star.h"

void z_star_init(ZStar* Star)
{
    Star->x = (int8_t)(rand() % S_WIDTH);
    Star->y = 0;
    Star->speed = (int8_t)(1 + (rand() % (Z_STAR_AVG_SPEED * 2 - 1)));
}

bool z_star_tick(ZPoolObject* Star)
{
    ZStar* star = (ZStar*)Star;

    star->y = (int8_t)(star->y + star->speed);

    return star->y >= S_HEIGHT;
}

void z_star_draw(ZPoolObject* Star)
{
    ZStar* star = (ZStar*)Star;

    s_draw_pixel(star->x, star->y, true);
}

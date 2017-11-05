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

void z_sprite_blitCentered(ZSprite* Sprite, int8_t X, int8_t Y, uint8_t Frame)
{
    z_sprite_blit(Sprite,
                  (int8_t)(X - z_sprite_getWidth(Sprite) / 2),
                  (int8_t)(Y - z_sprite_getHeight(Sprite) / 2),
                  Frame);
}

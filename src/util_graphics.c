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
#include "util_graphics.h"
#include "util_screen.h"
#include "data_gfx_asteroid.h"
#include "data_gfx_enemy00.h"
#include "data_gfx_enemy01.h"
#include "data_gfx_enemy02.h"
#include "data_gfx_player.h"
#include "data_gfx_player_left.h"
#include "data_gfx_player_right.h"
#include "data_gfx_player_forward.h"
#include "data_gfx_player_forward_left.h"
#include "data_gfx_player_forward_right.h"
#include "data_gfx_player_back.h"
#include "data_gfx_player_back_left.h"
#include "data_gfx_player_back_right.h"
#include "data_gfx_shield.h"
#include "data_gfx_hearts.h"

ZGraphics z_graphics;

void z_graphics_setup(void)
{
    z_sprite_load(&z_graphics.enemy[Z_SPRITE_ID_ASTEROID], asteroid);
    z_sprite_load(&z_graphics.enemy[Z_SPRITE_ID_ENEMY0], enemy00);
    z_sprite_load(&z_graphics.enemy[Z_SPRITE_ID_ENEMY1], enemy01);
    z_sprite_load(&z_graphics.enemy[Z_SPRITE_ID_ENEMY2], enemy02);
    z_sprite_load(&z_graphics.player[Z_BIT_RESTING], player);
    z_sprite_load(&z_graphics.player[Z_BIT_LEFT], player_left);
    z_sprite_load(&z_graphics.player[Z_BIT_RIGHT], player_right);
    z_sprite_load(&z_graphics.player[Z_BIT_FORWARD], player_forward);
    z_sprite_load(&z_graphics.player[Z_BIT_FORWARD | Z_BIT_LEFT], player_forward_left);
    z_sprite_load(&z_graphics.player[Z_BIT_FORWARD | Z_BIT_RIGHT], player_forward_right);
    z_sprite_load(&z_graphics.player[Z_BIT_BACK], player_back);
    z_sprite_load(&z_graphics.player[Z_BIT_BACK | Z_BIT_LEFT], player_back_left);
    z_sprite_load(&z_graphics.player[Z_BIT_BACK | Z_BIT_RIGHT], player_back_right);
    z_sprite_load(&z_graphics.hearts, hearts);
    z_sprite_load(&z_graphics.shield, shield);
}

void z_graphics_drawJets(uint8_t Sprite, int8_t X, int8_t Y)
{
    int8_t x, y, w, h;

    switch(Sprite) {
        case Z_SPRITE_ID_ENEMY0: {
            x = X;
            y = (int8_t)(Y - 2);
            z_draw_pixel(x, y, Z_COLOR_RED);
        } break;

        case Z_SPRITE_ID_ENEMY1: {
            x = (int8_t)(X - 3);
            y = (int8_t)(Y - 3);
            z_draw_pixel(x, y, Z_COLOR_RED);

            x = (int8_t)(X + 3);
            z_draw_pixel(x, y, Z_COLOR_RED);
        } break;

        case Z_SPRITE_ID_ENEMY2: {
            x = (int8_t)(X - 2);
            y = (int8_t)(Y - 4);
            w = 2;
            h = 1;
            z_draw_rectangle(x,
                             (int8_t)(y + z_screen_yShake),
                             w,
                             h,
                             Z_COLOR_RED);

            x = (int8_t)(X + 1);
            z_draw_rectangle(x,
                             (int8_t)(y + z_screen_yShake),
                             w,
                             h,
                             Z_COLOR_RED);
        } break;

        default: return;
    }
}

void z_sprite_blitCentered(ZSprite* Sprite, int8_t X, int8_t Y, uint8_t Frame)
{
    z_sprite_blit(Sprite,
                  (int8_t)(X - z_sprite_getWidth(Sprite) / 2),
                  (int8_t)(Y - z_sprite_getHeight(Sprite) / 2),
                  Frame);
}

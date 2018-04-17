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
#include "util_collision.h"

#include "obj_enemy.h"
#include "obj_player.h"
#include "util_effects.h"
#include "util_pool.h"
#include "util_screen.h"

typedef struct {
    int16_t x, y;
    int8_t w, h;
    uint8_t damage : 3;
    bool hit : 1;
} ZCollisionContext;

static bool boxAndBox(int16_t X1, int16_t Y1, int8_t W1, int8_t H1, int16_t X2, int16_t Y2, int8_t W2, int8_t H2)
{
    X1 = i16(X1 - W1 / 2);
    Y1 = i16(Y1 - H1 / 2);

    X2 = i16(X2 - W2 / 2);
    Y2 = i16(Y2 - H2 / 2);

    return !(Y1 >= Y2 + H2 || Y2 >= Y1 + H1 || X1 >= X2 + W2 || X2 >= X1 + W1);
}

static bool enemyShipCollision(ZPoolObjHeader* Enemy, void* Context)
{
    ZCollisionContext* context = Context;

    if(context->hit) {
        return true;
    }

    ZEnemy* enemy = (ZEnemy*)Enemy;

    if(boxAndBox(context->x,
                 context->y,
                 context->w,
                 context->h,
                 z_fix_toInt(enemy->x),
                 z_fix_toInt(enemy->y),
                 z_enemy_data[enemy->typeId].w,
                 z_enemy_data[enemy->typeId].h)) {

        context->hit = true;
        z_effect_particles(enemy->x, enemy->y, 4);
        z_enemy_takeDamage(enemy, context->damage);

        if(enemy->health > 0) {
            z_player_scorePoints(Z_POINTS_ENEMY_HIT);
        } else {
            z_player_scorePoints(Z_POINTS_ENEMY_DESTROYED);
        }
    }

    return enemy->health > 0;
}

bool z_collision_checkEnemyShips(ZFix X, ZFix Y, int8_t W, int8_t H, uint8_t Damage)
{
    ZCollisionContext context = {
        z_fix_toInt(X), z_fix_toInt(Y), W, H, u3(Damage), false
    };

    z_pool_tick(Z_POOL_ENEMY, enemyShipCollision, &context);

    return context.hit;
}

bool z_collision_checkPlayer(ZFix X, ZFix Y, int8_t W, int8_t H, uint8_t Damage)
{
    int8_t playerW = Z_PLAYER_W_NORMAL;
    int8_t playerH = Z_PLAYER_H_NORMAL;

    if(z_player.invincible) {
        playerW = Z_PLAYER_W_SHIELD;
        playerH = Z_PLAYER_H_SHIELD;
    }

    bool hit = boxAndBox(z_fix_toInt(X),
                         z_fix_toInt(Y),
                         W,
                         H,
                         z_fix_toInt(z_player.x),
                         z_fix_toInt(z_player.y),
                         playerW,
                         playerH);

    if(hit) {
        z_player_takeDamage(Damage);
        z_screen_shake(1);
        z_effect_particles(X, Y, 4);
    }

    return hit;
}

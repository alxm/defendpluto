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
#include "util_fix.h"
#include "util_collision.h"
#include "util_fps.h"
#include "util_pool.h"
#include "util_screen.h"
#include "obj_enemy.h"
#include "obj_particle.h"
#include "obj_player.h"

typedef struct {
    int16_t x, y;
    int8_t w, h;
    uint8_t damage;
    bool hit;
} ZCollisionContext;

static bool enemyShipCollision(ZPoolObject* Enemy, void* Context)
{
    ZCollisionContext* context = Context;

    if(context->hit) {
        return true;
    }

    ZEnemy* enemy = (ZEnemy*)Enemy;

    if(z_collision_boxAndBox(context->x,
                             context->y,
                             context->w,
                             context->h,
                             z_fix_fixtoi(enemy->x),
                             z_fix_fixtoi(enemy->y),
                             z_enemy_data[enemy->typeId].w,
                             z_enemy_data[enemy->typeId].h)) {

        context->hit = true;

        for(int8_t i = 4; i--; ) {
            ZParticle* p = z_pool_alloc(Z_POOL_PARTICLE);

            if(p == NULL) {
                break;
            }

            z_particle_init(p, enemy->x, enemy->y);
        }

        z_enemy_takeDamage(enemy, context->damage);
    }

    return enemy->health > 0;
}

bool z_collision_checkEnemyShips(ZFix X, ZFix Y, int8_t W, int8_t H, uint8_t Damage)
{
    ZCollisionContext context = {
        z_fix_fixtoi(X), z_fix_fixtoi(Y), W, H, Damage, false
    };

    z_pool_tick(Z_POOL_ENEMY, enemyShipCollision, &context);

    return context.hit;
}

bool z_collision_checkPlayer(ZFix X, ZFix Y, int8_t W, int8_t H, uint8_t Damage)
{
    bool hit = z_collision_boxAndBox(z_fix_fixtoi(X),
                                     z_fix_fixtoi(Y),
                                     W,
                                     H,
                                     z_fix_fixtoi(z_player.x),
                                     z_fix_fixtoi(z_player.y),
                                     z_player.w,
                                     z_player.h);

    if(hit) {
        z_player_takeDamage(Damage);
        z_screen_shake(Z_DS_TO_FRAMES(1));

        for(int8_t i = 4; i--; ) {
            ZParticle* p = z_pool_alloc(Z_POOL_PARTICLE);

            if(p == NULL) {
                break;
            }

            z_particle_init(p, X, Y);
        }
    }

    return hit;
}

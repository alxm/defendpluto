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
#include "util_random.h"
#include "util_screen.h"
#include "obj_bullet.h"
#include "obj_circle.h"
#include "obj_enemy.h"
#include "obj_particle.h"

static bool g_hit;
static ZBullet* g_bullet;

static bool pointInBox(int8_t X, int8_t Y, int8_t BoxX, int8_t BoxY, int8_t BoxW, int8_t BoxH)
{
    return X >= BoxX && X < BoxX + BoxW && Y >= BoxY && Y < BoxY + BoxH;
}

static bool checkBulletEnemyCollision(ZPoolObject* Enemy)
{
    if(g_hit) {
        return true;
    }

    ZEnemy* enemy = (ZEnemy*)Enemy;

    g_hit = pointInBox(z_fix_fixtoi(g_bullet->x),
                       z_fix_fixtoi(g_bullet->y),
                       (int8_t)(z_fix_fixtoi(enemy->x) - 4),
                       (int8_t)(z_fix_fixtoi(enemy->y) - 4),
                       8,
                       8);

    if(g_hit) {
        for(int8_t i = Z_PARTICLES_NUM; i--; ) {
            ZParticle* p = z_pool_alloc(Z_POOL_PARTICLE);

            if(p == NULL) {
                break;
            }

            z_particle_init(p,
                            enemy->x,
                            enemy->y,
                            (uint8_t)(Z_FPS / 8
                                        + z_random_uint8(Z_FPS / 4)));
        }

        ZCircle* c = z_pool_alloc(Z_POOL_CIRCLE);

        if(c) {
            z_circle_init(c,
                          z_fix_fixtoi(enemy->x),
                          z_fix_fixtoi(enemy->y),
                          5,
                          Z_FIX_ONE);
        }

        c = z_pool_alloc(Z_POOL_CIRCLE);

        if(c) {
            z_circle_init(c,
                          z_fix_fixtoi(enemy->x),
                          z_fix_fixtoi(enemy->y),
                          2,
                          Z_FIX_ONE / 2);
        }

        z_screen_shake(Z_FPS / 3);
    }

    return !g_hit;
}

void z_bullet_init(ZBullet* Bullet, ZFix X, ZFix Y, ZFix Dy)
{
    Bullet->x = X;
    Bullet->y = Y;
    Bullet->dy = Dy;
}

bool z_bullet_tick(ZPoolObject* Bullet)
{
    ZBullet* bullet = (ZBullet*)Bullet;

    bullet->y = (ZFix)(bullet->y + bullet->dy);

    if((bullet->dy < 0 && bullet->y < 0)
        || (bullet->dy > 0 && z_fix_fixtoi(bullet->y) >= Z_HEIGHT)) {

        return false;
    }

    g_hit = false;
    g_bullet = bullet;
    z_pool_tick(Z_POOL_ENEMY, checkBulletEnemyCollision);

    return !g_hit;
}

void z_bullet_draw(ZPoolObject* Bullet)
{
    ZBullet* bullet = (ZBullet*)Bullet;

    int8_t x = z_fix_fixtoi(bullet->x);
    int8_t y = z_fix_fixtoi(bullet->y);

    z_draw_rectangle((int8_t)(x - 1 + z_screen_xShake),
                     (int8_t)(y - 2 + z_screen_yShake),
                     2,
                     4,
                     Z_COLOR_RED);
}

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
#include "obj_bullet.h"
#include "obj_circle.h"
#include "obj_enemy.h"
#include "obj_particle.h"
#include "obj_star.h"

struct ZPool {
    ZPoolObject* freeList;
    ZPoolObject* activeList;
    uint8_t numActive;
    ZPoolObject pool[];
};

#define DECLARE_POOL(ObjectType, NumObjects)                              \
    union {                                                               \
        ZPool generic;                                                    \
        uint8_t private[sizeof(ZPool) + NumObjects * sizeof(ObjectType)]; \
    }

static DECLARE_POOL(ZStar, Z_STARS_NUM) g_starPool;
static DECLARE_POOL(ZBullet, Z_BULLETS_NUM) g_bulletPool;
static DECLARE_POOL(ZEnemy, Z_ENEMIES_NUM) g_enemyPool;
static DECLARE_POOL(ZCircle, Z_CIRCLES_NUM) g_circlePool;
static DECLARE_POOL(ZParticle, Z_PARTICLES_NUM) g_particlePool;

static ZPool* g_pools[Z_POOL_NUM] = {
    &g_starPool.generic,
    &g_bulletPool.generic,
    &g_enemyPool.generic,
    &g_circlePool.generic,
    &g_particlePool.generic,
};

static void initPool(ZPoolType Pool, size_t ObjectSize, size_t NumObjects)
{
    ZPool* pool = g_pools[Pool];
    ZPoolObject* current = &pool->pool[0];

    while(NumObjects-- > 1) {
        ZPoolObject* next = (void*)((uint8_t*)current + ObjectSize);
        current->next = next;
        current = next;
    }

    current->next = NULL;
    pool->freeList = &pool->pool[0];
    pool->activeList = NULL;
    pool->numActive = 0;
}

void z_pool_setup(void)
{
    initPool(Z_POOL_STAR, sizeof(ZStar), Z_STARS_NUM);
    initPool(Z_POOL_BULLET, sizeof(ZBullet), Z_BULLETS_NUM);
    initPool(Z_POOL_ENEMY, sizeof(ZEnemy), Z_ENEMIES_NUM);
    initPool(Z_POOL_CIRCLE, sizeof(ZCircle), Z_CIRCLES_NUM);
    initPool(Z_POOL_PARTICLE, sizeof(ZParticle), Z_PARTICLES_NUM);
}

void* z_pool_alloc(ZPoolType Pool)
{
    ZPool* pool = g_pools[Pool];

    if(pool->freeList == NULL) {
        return NULL;
    }

    ZPoolObject* object = pool->freeList;
    pool->freeList = pool->freeList->next;

    object->next = pool->activeList;
    pool->activeList = object;

    pool->numActive++;

    return object;
}

static void* z_pool_release(ZPool* Pool, void* Object, void* LastObject)
{
    ZPoolObject* object = Object;
    ZPoolObject* lastObject = LastObject;
    ZPoolObject* nextObject = object->next;

    if(lastObject == NULL) {
        Pool->activeList = nextObject;
    } else {
        lastObject->next = nextObject;
    }

    object->next = Pool->freeList;
    Pool->freeList = object;

    Pool->numActive--;

    return nextObject;
}

uint8_t z_pool_getNumActive(ZPoolType Pool)
{
    return g_pools[Pool]->numActive;
}

void z_pool_tick(ZPoolType Pool, bool (*Callback)(ZPoolObject*))
{
    ZPool* pool = g_pools[Pool];
    ZPoolObject* last = NULL;

    for(ZPoolObject* o = pool->activeList; o != NULL; ) {
        if(Callback(o)) {
            last = o;
            o = o->next;
        } else {
            o = z_pool_release(pool, o, last);
        }
    }
}

void z_pool_draw(ZPoolType Pool, void (*Callback)(ZPoolObject*))
{
    ZPool* pool = g_pools[Pool];

    for(ZPoolObject* o = pool->activeList; o != NULL; o = o->next) {
        Callback(o);
    }
}

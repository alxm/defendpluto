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
#include "util_enemy.h"
#include "util_fix.h"
#include "util_pool.h"
#include "obj_bullete.h"
#include "obj_bulletp.h"
#include "obj_circle.h"
#include "obj_enemy.h"
#include "obj_particle.h"
#include "obj_star.h"

struct ZPool {
    ZPoolObject* freeList;
    ZPoolObject* activeList;
    ZPoolObject pool[];
};

#define DECLARE_POOL(ObjectType, NumObjects)                              \
    union {                                                               \
        ZPool generic;                                                    \
        uint8_t private[sizeof(ZPool) + NumObjects * sizeof(ObjectType)]; \
    }

static DECLARE_POOL(ZStar, Z_STAR_POOL_NUM) g_starPool;
static DECLARE_POOL(ZBulletE, Z_BULLETE_POOL_NUM) g_bulletEPool;
static DECLARE_POOL(ZBulletP, Z_BULLETP_POOL_NUM) g_bulletPPool;
static DECLARE_POOL(ZEnemy, Z_ENEMY_POOL_NUM) g_enemyPool;
static DECLARE_POOL(ZCircle, Z_CIRCLE_POOL_NUM) g_circlePool;
static DECLARE_POOL(ZParticle, Z_PARTICLE_POOL_NUM) g_particlePool;

static ZPool* g_pools[Z_POOL_NUM] = {
    &g_starPool.generic,
    &g_bulletEPool.generic,
    &g_bulletPPool.generic,
    &g_enemyPool.generic,
    &g_circlePool.generic,
    &g_particlePool.generic,
};

static void initPool(uint8_t Pool, size_t ObjectSize, size_t NumObjects)
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
}

void z_pool_setup(void)
{
    z_pool_reset();
}

void z_pool_reset(void)
{
    initPool(Z_POOL_STAR, sizeof(ZStar), Z_STAR_POOL_NUM);
    initPool(Z_POOL_BULLETE, sizeof(ZBulletE), Z_BULLETE_POOL_NUM);
    initPool(Z_POOL_BULLETP, sizeof(ZBulletP), Z_BULLETP_POOL_NUM);
    initPool(Z_POOL_ENEMY, sizeof(ZEnemy), Z_ENEMY_POOL_NUM);
    initPool(Z_POOL_CIRCLE, sizeof(ZCircle), Z_CIRCLE_POOL_NUM);
    initPool(Z_POOL_PARTICLE, sizeof(ZParticle), Z_PARTICLE_POOL_NUM);
}

void* z_pool_alloc(uint8_t Pool)
{
    ZPool* pool = g_pools[Pool];

    if(pool->freeList == NULL) {
        return NULL;
    }

    ZPoolObject* object = pool->freeList;
    pool->freeList = pool->freeList->next;

    object->next = pool->activeList;
    pool->activeList = object;

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

    return nextObject;
}

bool z_pool_noActive(uint8_t Pool)
{
    return g_pools[Pool]->activeList == NULL;
}

void z_pool_tick(uint8_t Pool, bool (*Callback)(ZPoolObject*))
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

void z_pool_draw(uint8_t Pool, void (*Callback)(ZPoolObject*))
{
    ZPool* pool = g_pools[Pool];

    for(ZPoolObject* o = pool->activeList; o != NULL; o = o->next) {
        Callback(o);
    }
}

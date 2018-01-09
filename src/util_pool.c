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
#include "util_pool.h"
#include "obj_bullete.h"
#include "obj_bulletp.h"
#include "obj_circle.h"
#include "obj_enemy.h"
#include "obj_particle.h"
#include "obj_star.h"

#define Z_OFFSET_NULL ((ZPoolOffset)-1)

struct ZPool {
    ZPoolOffset freeList;
    ZPoolOffset activeList;
    ZPoolObject pool[];
};

#define DECLARE_POOL(ObjectType, NumObjects)                              \
    union {                                                               \
        ZPool generic;                                                    \
        uint8_t private[sizeof(ZPool) + NumObjects * sizeof(ObjectType)]; \
    }

static DECLARE_POOL(ZStar, Z_POOL_NUM_STAR) g_starPool;
static DECLARE_POOL(ZBulletE, Z_POOL_NUM_BULLETE) g_bulletEPool;
static DECLARE_POOL(ZBulletP, Z_POOL_NUM_BULLETP) g_bulletPPool;
static DECLARE_POOL(ZEnemy, Z_POOL_NUM_ENEMY) g_enemyPool;
static DECLARE_POOL(ZCircle, Z_POOL_NUM_CIRCLE) g_circlePool;
static DECLARE_POOL(ZParticle, Z_POOL_NUM_PARTICLE) g_particlePool;

static ZPool* g_pools[Z_POOL_NUM] = {
    &g_starPool.generic,
    &g_bulletEPool.generic,
    &g_bulletPPool.generic,
    &g_enemyPool.generic,
    &g_circlePool.generic,
    &g_particlePool.generic,
};

static inline ZPoolObject* objectFromOffset(ZPool* Pool, ZPoolOffset Offset)
{
    return (ZPoolObject*)(void*)((uint8_t*)Pool->pool + Offset);
}

static inline ZPoolOffset offsetFromObject(ZPool* Pool, ZPoolObject* Object)
{
    return zpo((uint8_t*)Object - (uint8_t*)Pool->pool);
}

static void initPool(uint8_t Pool, uint8_t ObjectSize, uint8_t NumObjects)
{
    ZPool* pool = g_pools[Pool];
    ZPoolObject* current = &pool->pool[0];

    while(NumObjects-- > 1) {
        ZPoolObject* next = (void*)((uint8_t*)current + ObjectSize);

        current->nextOffset = offsetFromObject(pool, next);
        current = next;
    }

    current->nextOffset = Z_OFFSET_NULL;
    pool->freeList = 0;
    pool->activeList = Z_OFFSET_NULL;
}

void z_pool_setup(void)
{
    initPool(Z_POOL_STAR, sizeof(ZStar), Z_POOL_NUM_STAR);
    initPool(Z_POOL_BULLETE, sizeof(ZBulletE), Z_POOL_NUM_BULLETE);
    initPool(Z_POOL_BULLETP, sizeof(ZBulletP), Z_POOL_NUM_BULLETP);
    initPool(Z_POOL_ENEMY, sizeof(ZEnemy), Z_POOL_NUM_ENEMY);
    initPool(Z_POOL_CIRCLE, sizeof(ZCircle), Z_POOL_NUM_CIRCLE);
    initPool(Z_POOL_PARTICLE, sizeof(ZParticle), Z_POOL_NUM_PARTICLE);
}

void z_pool_reset(void)
{
    initPool(Z_POOL_BULLETE, sizeof(ZBulletE), Z_POOL_NUM_BULLETE);
    initPool(Z_POOL_BULLETP, sizeof(ZBulletP), Z_POOL_NUM_BULLETP);
    initPool(Z_POOL_ENEMY, sizeof(ZEnemy), Z_POOL_NUM_ENEMY);
    initPool(Z_POOL_CIRCLE, sizeof(ZCircle), Z_POOL_NUM_CIRCLE);
    initPool(Z_POOL_PARTICLE, sizeof(ZParticle), Z_POOL_NUM_PARTICLE);
}

void* z_pool_alloc(uint8_t Pool)
{
    ZPool* pool = g_pools[Pool];

    if(pool->freeList == Z_OFFSET_NULL) {
        #if Z_DEBUG_STATS && A_PLATFORM_SYSTEM_DESKTOP
            static uint32_t fails[Z_POOL_NUM];
            static const char* names[Z_POOL_NUM] = {
                "Z_POOL_STAR",
                "Z_POOL_BULLETE",
                "Z_POOL_BULLETP",
                "Z_POOL_ENEMY",
                "Z_POOL_CIRCLE",
                "Z_POOL_PARTICLE",
            };

            printf("Can't allocate from %s (%d)\n", names[Pool], ++fails[Pool]);
        #endif
        return NULL;
    }

    ZPoolOffset objectOffset = pool->freeList;
    ZPoolObject* object = objectFromOffset(pool, objectOffset);

    pool->freeList = object->nextOffset;
    object->nextOffset = pool->activeList;
    pool->activeList = objectOffset;

    return object;
}

static ZPoolOffset z_pool_release(ZPool* Pool, void* Object, ZPoolOffset ObjectOffset, void* LastObject)
{
    ZPoolObject* object = Object;
    ZPoolObject* lastObject = LastObject;
    ZPoolOffset nextObjectOffset = object->nextOffset;

    if(lastObject == NULL) {
        Pool->activeList = nextObjectOffset;
    } else {
        lastObject->nextOffset = nextObjectOffset;
    }

    object->nextOffset = Pool->freeList;
    Pool->freeList = ObjectOffset;

    return nextObjectOffset;
}

void z_pool_clear(uint8_t Pool)
{
    ZPool* pool = g_pools[Pool];

    for(ZPoolOffset offset = pool->activeList; offset != Z_OFFSET_NULL; ) {
        ZPoolObject* obj = objectFromOffset(pool, offset);
        offset = z_pool_release(pool, obj, offset, NULL);
    }
}

bool z_pool_noActive(uint8_t Pool)
{
    return g_pools[Pool]->activeList == Z_OFFSET_NULL;
}

void z_pool_tick(uint8_t Pool, ZPoolTickCallback* Callback, void* Context)
{
    ZPool* pool = g_pools[Pool];
    ZPoolObject* lastObj = NULL;

    for(ZPoolOffset offset = pool->activeList; offset != Z_OFFSET_NULL; ) {
        ZPoolObject* obj = objectFromOffset(pool, offset);

        if(Callback(obj, Context)) {
            lastObj = obj;
            offset = obj->nextOffset;
        } else {
            offset = z_pool_release(pool, obj, offset, lastObj);
        }
    }
}

void z_pool_draw(uint8_t Pool, void (*Callback)(ZPoolObject*))
{
    ZPool* pool = g_pools[Pool];

    for(ZPoolOffset offset = pool->activeList; offset != Z_OFFSET_NULL; ) {
        ZPoolObject* obj = objectFromOffset(pool, offset);

        Callback(obj);
        offset = obj->nextOffset;
    }
}

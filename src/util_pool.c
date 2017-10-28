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
#include "obj_bullet.h"
#include "obj_enemy.h"
#include "obj_star.h"

struct ZPool {
    ZPoolObject* freeList;
    ZPoolObject* activeList;
    ZPoolObject pool[];
};

#define DECLARE(ObjectType, NumObjects)                                   \
    union {                                                               \
        ZPool generic;                                                    \
        uint8_t private[sizeof(ZPool) + NumObjects * sizeof(ObjectType)]; \
    }

static DECLARE(ZStar, Z_STARS_NUM) g_starPool;
static DECLARE(ZBullet, Z_BULLETS_NUM) g_bulletPool;
static DECLARE(ZEnemy, Z_ENEMIES_NUM) g_enemyPool;

ZPool* z_pool[Z_POOL_NUM] = {
    &g_starPool.generic,
    &g_bulletPool.generic,
    &g_enemyPool.generic,
};

static void init(ZPool* Pool, size_t ObjectSize, size_t NumObjects)
{
    ZPoolObject* current = &Pool->pool[0];

    while(NumObjects-- > 1) {
        ZPoolObject* next = (void*)((uint8_t*)current + ObjectSize);
        current->next = next;
        current = next;
    }

    current->next = NULL;
    Pool->freeList = &Pool->pool[0];
    Pool->activeList = NULL;
}

void z_pool_setup(void)
{
    init(z_pool[Z_POOL_STAR], sizeof(ZStar), Z_STARS_NUM);
    init(z_pool[Z_POOL_BULLET], sizeof(ZBullet), Z_BULLETS_NUM);
    init(z_pool[Z_POOL_ENEMY], sizeof(ZEnemy), Z_ENEMIES_NUM);
}

void* z_pool_alloc(ZPool* Pool)
{
    ZPool* pool = Pool;

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
    ZPool* pool = Pool;

    ZPoolObject* object = Object;
    ZPoolObject* lastObject = LastObject;
    ZPoolObject* nextObject = object->next;

    if(lastObject == NULL) {
        pool->activeList = nextObject;
    } else {
        lastObject->next = nextObject;
    }

    object->next = pool->freeList;
    pool->freeList = object;

    return nextObject;
}

void z_pool_tick(ZPool* Pool, bool (*Callback)(ZPoolObject*))
{
    ZPoolObject* last = NULL;

    for(ZPoolObject* o = Pool->activeList; o != NULL; ) {
        if(Callback(o)) {
            o = z_pool_release(Pool, o, last);
        } else {
            last = o;
            o = o->next;
        }
    }
}

void z_pool_draw(ZPool* Pool, void (*Callback)(ZPoolObject*))
{
    for(ZPoolObject* o = Pool->activeList; o != NULL; o = o->next) {
        Callback(o);
    }
}

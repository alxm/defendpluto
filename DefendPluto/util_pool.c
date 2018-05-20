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
#include "util_pool.h"

static ZPoolHeader* g_pools[Z_POOL_NUM];
static const ZPoolObjOffset Z_OFFSET_NULL = (ZPoolObjOffset)-1;

static inline ZPoolObjHeader* objectFromOffset(ZPoolHeader* Pool, ZPoolObjOffset Offset)
{
    return (ZPoolObjHeader*)((uint8_t*)(Pool + 1) + Offset);
}

static inline ZPoolObjOffset offsetFromObject(ZPoolHeader* Pool, ZPoolObjHeader* Object)
{
    return zpo((uint8_t*)Object - (uint8_t*)(Pool + 1));
}

void z_pool_register(ZPoolId Id, ZPoolHeader* Pool)
{
    g_pools[Id] = Pool;
    z_pool_reset(Id);
}

void z_pool_reset(ZPoolId Pool)
{
    ZPoolHeader* pool = g_pools[Pool];
    ZPoolObjHeader* current = (ZPoolObjHeader*)(pool + 1);
    ZPoolObjOffset numObjects = pool->capacity;

    while(numObjects-- > 1) {
        ZPoolObjHeader* next = (void*)((uint8_t*)current + pool->objSize);

        current->nextOffset = offsetFromObject(pool, next);
        current = next;
    }

    current->nextOffset = Z_OFFSET_NULL;
    pool->freeList = 0;
    pool->activeList = Z_OFFSET_NULL;
}

void* z_pool_alloc(ZPoolId Pool)
{
    ZPoolHeader* pool = g_pools[Pool];

    if(pool->freeList == Z_OFFSET_NULL) {
        #if Z_DEBUG_STATS && A_PLATFORM_SYSTEM_DESKTOP
            static uint32_t fails[Z_POOL_NUM];
            static const char* names[Z_POOL_NUM] = {
                [Z_POOL_STAR] = "Z_POOL_STAR",
                [Z_POOL_BULLETE] = "Z_POOL_BULLETE",
                [Z_POOL_BULLETP] = "Z_POOL_BULLETP",
                [Z_POOL_ENEMY] = "Z_POOL_ENEMY",
                [Z_POOL_CIRCLE] = "Z_POOL_CIRCLE",
                [Z_POOL_PARTICLE] = "Z_POOL_PARTICLE",
            };

            printf("Can't allocate from %s (%d)\n", names[Pool], ++fails[Pool]);
        #endif
        return NULL;
    }

    ZPoolObjOffset objectOffset = pool->freeList;
    ZPoolObjHeader* object = objectFromOffset(pool, objectOffset);

    pool->freeList = object->nextOffset;
    object->nextOffset = pool->activeList;
    pool->activeList = objectOffset;

    return object;
}

static ZPoolObjOffset releaseObject(ZPoolHeader* Pool, void* Object, ZPoolObjOffset ObjectOffset, void* LastObject)
{
    ZPoolObjHeader* object = Object;
    ZPoolObjHeader* lastObject = LastObject;
    ZPoolObjOffset nextObjectOffset = object->nextOffset;

    if(lastObject == NULL) {
        Pool->activeList = nextObjectOffset;
    } else {
        lastObject->nextOffset = nextObjectOffset;
    }

    object->nextOffset = Pool->freeList;
    Pool->freeList = ObjectOffset;

    return nextObjectOffset;
}

void z_pool_clear(ZPoolId Pool)
{
    ZPoolHeader* pool = g_pools[Pool];

    for(ZPoolObjOffset offset = pool->activeList; offset != Z_OFFSET_NULL; ) {
        ZPoolObjHeader* obj = objectFromOffset(pool, offset);
        offset = releaseObject(pool, obj, offset, NULL);
    }
}

bool z_pool_noActive(ZPoolId Pool)
{
    return g_pools[Pool]->activeList == Z_OFFSET_NULL;
}

void z_pool_tick(ZPoolId Pool, ZPoolTick* Callback, void* Context)
{
    ZPoolHeader* pool = g_pools[Pool];
    ZPoolObjHeader* lastObj = NULL;

    for(ZPoolObjOffset offset = pool->activeList; offset != Z_OFFSET_NULL; ) {
        ZPoolObjHeader* obj = objectFromOffset(pool, offset);

        if(Callback(obj, Context)) {
            lastObj = obj;
            offset = obj->nextOffset;
        } else {
            offset = releaseObject(pool, obj, offset, lastObj);
        }
    }
}

void z_pool_draw(ZPoolId Pool, ZPoolDraw* Callback)
{
    ZPoolHeader* pool = g_pools[Pool];

    for(ZPoolObjOffset offset = pool->activeList; offset != Z_OFFSET_NULL; ) {
        ZPoolObjHeader* obj = objectFromOffset(pool, offset);

        Callback(obj);
        offset = obj->nextOffset;
    }
}

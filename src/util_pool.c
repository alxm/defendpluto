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

typedef struct {
    ZPoolObject* freeList;
    ZPoolObject* activeList;
    ZPoolObject pool[];
} ZPool;

void z_pool_init(void* Pool, size_t ObjectSize, size_t NumObjects)
{
    ZPool* pool = Pool;

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

void* z_pool_alloc(void* Pool)
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

void* z_pool_release(void* Pool, void* Object, void* LastObject)
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

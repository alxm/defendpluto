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

typedef struct {
    void* next;
} ZPoolObject;

typedef struct {
    void* freeList;
    void* activeList;
    ZPoolObject pool[1];
} ZPool;

#define Z_POOL_DECLARE(ObjectType, NumObjects, Name) \
    union {                                          \
        struct {                                     \
            ObjectType* freeList;                    \
            ObjectType* activeList;                  \
            ObjectType pool[NumObjects];             \
        } Name;                                      \
        ZPool generic;                               \
    }

extern void z_pool_init(ZPool* Pool, size_t ObjectSize, size_t NumObjects);
extern void* z_pool_alloc(ZPool* Pool);
extern void* z_pool_release(ZPool* Pool, void* Object, void* LastObject);

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

typedef enum {
    Z_POOL_STAR,
    Z_POOL_BULLET,
    Z_POOL_ENEMY,
    Z_POOL_NUM
} ZPoolType;

typedef struct {
    void* next;
} ZPoolObject;

typedef struct {
    ZPoolObject* freeList;
    ZPoolObject* activeList;
    ZPoolObject pool[1];
} ZPool;

extern void z_pool_setup(void);
extern ZPool* z_pool_get(ZPoolType Type);

extern void* z_pool_alloc(ZPool* Pool);
extern void* z_pool_release(ZPool* Pool, void* Object, void* LastObject);

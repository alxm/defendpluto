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

typedef enum {
    Z_POOL_INVALID = -1,
    Z_POOL_STAR,
    Z_POOL_BULLETE,
    Z_POOL_BULLETP,
    Z_POOL_ENEMY,
    Z_POOL_CIRCLE,
    Z_POOL_PARTICLE,
    Z_POOL_NUM
} ZPoolType;

#define Z_POOL_NUM_BULLETE 8
#define Z_POOL_NUM_BULLETP 3
#define Z_POOL_NUM_CIRCLE 2
#define Z_POOL_NUM_ENEMY 16
#define Z_POOL_NUM_PARTICLE 8
#define Z_POOL_NUM_STAR 16

typedef struct ZPool ZPool;

typedef struct {
    ZPoolOffset nextOffset;
} ZPoolObject;

typedef bool ZPoolTickCallback(ZPoolObject*, void*);
typedef void ZPoolDrawCallback(ZPoolObject*);

extern void z_pool_setup(void);
extern void z_pool_reset(void);

extern void* z_pool_alloc(uint8_t Pool);
extern bool z_pool_noActive(uint8_t Pool);

extern void z_pool_tick(uint8_t Pool, ZPoolTickCallback* Callback, void* Context);
extern void z_pool_draw(uint8_t Pool, void (*Callback)(ZPoolObject*));

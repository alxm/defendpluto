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
    Z_POOL_INVALID = -1,
    Z_POOL_STAR,
    Z_POOL_BULLETE,
    Z_POOL_BULLETP,
    Z_POOL_ENEMY,
    Z_POOL_CIRCLE,
    Z_POOL_PARTICLE,
    Z_POOL_NUM
} ZPoolType;

typedef struct ZPool ZPool;

typedef struct {
    void* next;
} ZPoolObject;

extern void z_pool_setup(void);
extern void z_pool_reset(void);

extern void* z_pool_alloc(uint8_t Pool);
extern bool z_pool_noActive(uint8_t Pool);

extern void z_pool_tick(uint8_t Pool, bool (*Callback)(ZPoolObject*));
extern void z_pool_draw(uint8_t Pool, void (*Callback)(ZPoolObject*));

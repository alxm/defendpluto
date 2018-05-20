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

#pragma once

typedef enum Z_ENUM_PACK {
    Z_POOL_INVALID = -1,
    Z_POOL_STAR,
    Z_POOL_BULLETE,
    Z_POOL_BULLETP,
    Z_POOL_ENEMY,
    Z_POOL_CIRCLE,
    Z_POOL_PARTICLE,
    Z_POOL_NUM
} ZPoolId;

typedef struct {
    ZPoolObjOffset nextOffset;
} ZPoolObjHeader;

typedef struct ZPoolHeader {
    ZPoolObjOffset freeList;
    ZPoolObjOffset activeList;
    const ZPoolObjOffset capacity;
    const ZPoolObjOffset objSize;
} ZPoolHeader;

#define Z_POOL_DECLARE(ObjectType, NumObjects, VarName)              \
    static struct {                                                  \
        ZPoolHeader header;                                          \
        uint8_t data[NumObjects * sizeof(ObjectType)];               \
    } z_pool__##ObjectType = {                                       \
        .header = {                                                  \
            .capacity = NumObjects,                                  \
            .objSize = sizeof(ObjectType)                            \
        }                                                            \
    };                                                               \
    static ZPoolHeader* const VarName = &z_pool__##ObjectType.header

typedef bool ZPoolTick(ZPoolObjHeader*, void*);
typedef void ZPoolDraw(ZPoolObjHeader*);

extern void z_pool_register(ZPoolId Id, ZPoolHeader* Pool);
extern void z_pool_reset(ZPoolId Pool);

extern void* z_pool_alloc(ZPoolId Pool);
extern void z_pool_clear(ZPoolId Pool);
extern bool z_pool_noActive(ZPoolId Pool);

extern void z_pool_tick(ZPoolId Pool, ZPoolTick* Callback, void* Context);
extern void z_pool_draw(ZPoolId Pool, ZPoolDraw* Callback);

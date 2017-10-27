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
    ZPoolObject poolObject;
    fix x, y;
    fix speed;
    int dy;
} ZBullet;

#define Z_BULLETS_NUM 32

extern void z_bullet_init(ZBullet* Bullet, fix X, fix Y, int Dy);
extern bool z_bullet_tick(ZBullet* Bullet);
extern void z_bullet_draw(ZBullet* Bullet);


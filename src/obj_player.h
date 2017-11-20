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

#define Z_SHIELD_MAX 64
#define Z_SHIELD_DAMAGE_COLLISION (Z_SHIELD_MAX / 2)
#define Z_SHIELD_DAMAGE_SHOT (Z_SHIELD_MAX / 4)
#define Z_SHIELD_DAMAGE_SHOOTING (Z_SHIELD_MAX / 16)
#define Z_SHIELD_BOOST_HEART (Z_SHIELD_MAX / 2)
#define Z_SHIELD_BOOST_REGEN (Z_SHIELD_MAX / Z_FPS)

typedef struct {
    ZFix x, y;
    int8_t dx, dy;
    uint8_t w : 4;
    uint8_t h : 4;
    uint8_t frame : 4;
    uint8_t shootShift : 1;
    bool jetFlicker : 1;
    uint8_t shield;
    int8_t health;
    uint8_t lastShotCounter : 7;
    bool heartsBlink : 1;
} ZPlayer;

extern ZPlayer z_player;

extern void z_player_init(int8_t X, int8_t Y);
extern void z_player_tick(void);
extern void z_player_draw(void);

extern void z_player_takeDamage(uint8_t Damage);

extern void z_player_hudTick(void);
extern void z_player_hudDraw(void);

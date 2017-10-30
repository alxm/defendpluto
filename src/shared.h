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

#ifdef ARDUINO
    #include <stdlib.h>
    #include <stdbool.h>
    #include <stdint.h>
#else
    #include <a2x.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define Z_UNUSED(X) (X = X)

#define Z_WIDTH 128
#define Z_HEIGHT 64
#define Z_FPS 30

#ifdef ARDUINO
    typedef uint8_t ZButton;
#else
    typedef AInputButton* ZButton;
#endif

typedef struct {
    ZButton up;
    ZButton down;
    ZButton left;
    ZButton right;
    ZButton a;
    ZButton b;
} ZControls;

extern ZControls z_controls;

extern void z_shared_setup(void);

extern unsigned z_fps_getCounter(void);
extern bool z_fps_isNthFrame(unsigned N);

extern bool z_button_pressed(ZButton Button);

extern void z_draw_fill(bool White);
extern void z_draw_rectangle(int X, int Y, int W, int H, bool White);
extern void z_draw_pixel(int X, int Y, bool White);

#ifdef __cplusplus
}
#endif

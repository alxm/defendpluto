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

#define S_WIDTH 128
#define S_HEIGHT 64
#define S_FPS 30

typedef enum {
    S_BUTTON_UP,
    S_BUTTON_DOWN,
    S_BUTTON_LEFT,
    S_BUTTON_RIGHT,
    S_BUTTON_A,
    S_BUTTON_B,
    S_BUTTON_NUM
} SButtonType;

#ifdef ARDUINO
    typedef uint8_t SButton;
#else
    typedef AInputButton* SButton;
#endif

extern SButton s_buttons[S_BUTTON_NUM];

extern void s_setup(void);

extern bool s_button_pressed(SButton Button);

extern void s_draw_fill(bool White);
extern void s_draw_rectangle(int X, int Y, int W, int H, bool White);
extern void s_draw_pixel(int X, int Y, bool White);

#ifdef __cplusplus
}
#endif
